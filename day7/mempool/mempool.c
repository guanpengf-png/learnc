/*
 * =============================================================================
 * 简易内存池 / 空闲块链表（教学版）
 * =============================================================================
 *
 * 练习重点：
 *   - 结构体 + 指针算术（块头紧挨用户数据）
 *   - 空闲块单链表（free_next）
 *   - 物理相邻双向链（phys_prev / phys_next）——方便合并
 *   - 分配：首次适应 + 分裂大块
 *   - 释放：链回空闲表 + 与左右空闲块合并（消碎片）
 *
 * 编译运行：
 *   cd day7/mempool && make && ./mempool
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define POOL_BYTES (1024) /* 整池大小：1KB，方便打印观察 */
#define MIN_SPLIT  16     /* 分裂后剩余太小就不切，避免碎渣 */

/* -----------------------------------------------------------------------------
 * 块头：每块内存前面都有这么一个「户口本」
 *
 * 内存布局（一块）：
 *   [Block 头][........ 用户可写的 payload ........]
 *             ^
 *             mempool_alloc 返回给用户的指针
 *
 * phys_*  ：按地址从左到右串起「整片池子里的所有块」（不管忙闲）
 * free_next：只把「空闲块」串成一条链表，分配时只在这条链上找
 * ----------------------------------------------------------------------------- */
typedef struct Block {
    size_t size;            /* payload 字节数（不含本头部） */
    int is_free;            /* 1=空闲，0=已分配给用户 */
    struct Block *phys_prev; /* 物理上左边相邻块；池首为 NULL */
    struct Block *phys_next; /* 物理上右边相邻块；池尾为 NULL */
    struct Block *free_next; /* 空闲链表的下一个；忙块时无不到 */
} Block;

/*
 * MemPool：一整片连续大缓冲 + 空闲链头
 * arena：真正的字节数组；所有 Block 都「镶嵌」在这片字节里
 */
typedef struct {
    unsigned char *arena; /* 池子底层存储 */
    size_t capacity;      /* arena 总字节数 */
    Block *free_head;     /* 空闲链表头 */
    Block *phys_head;     /* 物理第一块（通常也是最左块） */
} MemPool;

/* =============================================================================
 * 小工具：用户指针 <-> 块头
 * ============================================================================= */

/*
 * block_from_ptr —— 用户手里的指针往回退，找回块头
 *
 * 干什么：alloc 返回的是「头后面」的地址；free 时要能找到头。
 *   用户指针 p
 *   块头 = p - sizeof(Block)
 */
static Block *block_from_ptr(void *user_ptr)
{
    return (Block *)((unsigned char *)user_ptr - sizeof(Block));
}

/*
 * user_from_block —— 块头往后挪，得到用户指针
 */
static void *user_from_block(Block *b)
{
    return (unsigned char *)b + sizeof(Block);
}

/* =============================================================================
 * 空闲链表：头插 / 按地址摘除
 * ============================================================================= */

/*
 * freelist_push —— 把一块空闲块插到空闲链头部
 */
static void freelist_push(MemPool *pool, Block *b)
{
    b->is_free = 1;
    b->free_next = pool->free_head;
    pool->free_head = b;
}

/*
 * freelist_remove —— 从空闲链里摘掉指定块（不一定是头）
 *
 * 干什么：分配走了一块、或合并时被吃掉的块，要从空闲链消失。
 * 用二级思路：顺着 free_next 找，让前驱的 free_next 跳过它。
 */
static void freelist_remove(MemPool *pool, Block *target)
{
    Block *cur = pool->free_head;
    Block *prev = NULL;

    while (cur != NULL) {
        if (cur == target) {
            if (prev == NULL) {
                pool->free_head = cur->free_next;
            } else {
                prev->free_next = cur->free_next;
            }
            cur->free_next = NULL;
            return;
        }
        prev = cur;
        cur = cur->free_next;
    }
}

/* =============================================================================
 * 初始化 / 销毁
 * ============================================================================= */

/*
 * pool_init —— 向系统只要一次大内存，整池先做成「一整块空闲」
 *
 * 图示：
 *   arena: [======== Block(空闲, size≈整池-头) ========]
 *   free_head -> 这一块
 */
static int pool_init(MemPool *pool, size_t bytes)
{
    if (bytes < sizeof(Block) + MIN_SPLIT) {
        return 0;
    }

    pool->arena = (unsigned char *)malloc(bytes);
    if (!pool->arena) {
        return 0;
    }
    memset(pool->arena, 0, bytes);
    pool->capacity = bytes;

    /* 把 arena 起始处当作第一块的头 */
    Block *b = (Block *)pool->arena;
    b->size = bytes - sizeof(Block);
    b->is_free = 1;
    b->phys_prev = NULL;
    b->phys_next = NULL;
    b->free_next = NULL;

    pool->phys_head = b;
    pool->free_head = b;
    return 1;
}

static void pool_destroy(MemPool *pool)
{
    free(pool->arena);
    pool->arena = NULL;
    pool->free_head = NULL;
    pool->phys_head = NULL;
    pool->capacity = 0;
}

/* =============================================================================
 * 分裂：大空闲块切成「前段给用户」+「后段继续空闲」
 * ============================================================================= */

/*
 * block_split —— 若空闲块比 need 大很多，切出后面一小块留在空闲链
 *
 * 参数：
 *   b    —— 即将分给用户的空闲块（调用前已从需求上判定够大）
 *   need —— 用户真正需要的 payload 大小
 *
 * 干什么：
 *   原来： [头|........ size ........]
 *   切后： [头|..need..|头|..剩余..]
 *                 ^给用户   ^新空闲块
 *
 * 物理链、空闲链都要更新。
 */
static void block_split(MemPool *pool, Block *b, size_t need)
{
    /*
     * 剩余 payload = 原 size - need - 新块头大小
     * 只有剩余 >= MIN_SPLIT 才值得切，否则整块直接给用户（略浪费但简单）
     */
    size_t remain_payload = b->size - need - sizeof(Block);
    if (b->size < need + sizeof(Block) + MIN_SPLIT) {
        return; /* 不切 */
    }

    /* 新块头就放在「用户区 need 字节之后」 */
    unsigned char *split_at = (unsigned char *)user_from_block(b) + need;
    Block *n = (Block *)split_at;

    n->size = remain_payload;
    n->is_free = 1;
    n->phys_prev = b;
    n->phys_next = b->phys_next;
    n->free_next = NULL;

    if (b->phys_next) {
        b->phys_next->phys_prev = n;
    }
    b->phys_next = n;
    b->size = need;

    /* 新块是空闲的，挂进空闲链；b 马上要变成忙块，稍后从空闲链摘掉 */
    freelist_push(pool, n);
}

/* =============================================================================
 * 合并：释放后把相邻空闲块揉成更大一块（抗碎片）
 * ============================================================================= */

/*
 * block_coalesce —— 尝试把 b 和右边、左边的空闲邻居合并
 *
 * 规则：
 *   1. 若 phys_next 空闲：吞掉右边（size 加大，物理链跳过右边，空闲链摘掉右边）
 *   2. 若 phys_prev 空闲：让左边吞掉自己（返回合并后的那块，一般是左边）
 *
 * 返回：合并完成后「代表这块区域」的块指针（可能还是 b，也可能是左边）
 */
static Block *block_coalesce(MemPool *pool, Block *b)
{
    /* ---- 先合并右边 ---- */
    Block *right = b->phys_next;
    if (right != NULL && right->is_free) {
        freelist_remove(pool, right);

        b->size += sizeof(Block) + right->size;
        b->phys_next = right->phys_next;
        if (right->phys_next) {
            right->phys_next->phys_prev = b;
        }
        /* right 头已并进 b 的空间，不再单独存在 */
    }

    /* ---- 再合并左边：左边吞 b ---- */
    Block *left = b->phys_prev;
    if (left != NULL && left->is_free) {
        freelist_remove(pool, b);

        left->size += sizeof(Block) + b->size;
        left->phys_next = b->phys_next;
        if (b->phys_next) {
            b->phys_next->phys_prev = left;
        }
        /* b 已并进 left；若 b 在空闲链上需已摘掉。left 本就在空闲链 */
        return left;
    }

    return b;
}

/* =============================================================================
 * alloc / free（对外接口）
 * ============================================================================= */

/*
 * pool_alloc —— 从池里分配至少 n 字节
 *
 * 策略：首次适应（First Fit）
 *   从 free_head 开始找第一块 size >= n 的空闲块；
 *   可选分裂；标为忙碌；从空闲链摘掉；返回用户指针。
 *
 * 返回：成功为 payload 指针；失败为 NULL
 */
static void *pool_alloc(MemPool *pool, size_t n)
{
    if (n == 0) {
        return NULL;
    }

    Block *cur = pool->free_head;

    while (cur != NULL) {
        if (cur->is_free && cur->size >= n) {
            /*
             * 先分裂：余块会 freelist_push 到链头，
             * 此时 free 链已变，统一用 freelist_remove 摘掉 cur。
             */
            block_split(pool, cur, n);
            freelist_remove(pool, cur);
            cur->is_free = 0;

            return user_from_block(cur);
        }
        cur = cur->free_next;
    }

    return NULL; /* 没有足够大的空闲块 */
}

/*
 * pool_free —— 归还用户指针指向的内存
 *
 * 干什么：
 *   1. 找回块头，标为空闲
 *   2. 挂回空闲链
 *   3. coalesce 合并左右空闲邻居
 */
static void pool_free(MemPool *pool, void *user_ptr)
{
    if (!user_ptr) {
        return;
    }

    Block *b = block_from_ptr(user_ptr);

    /* 简单校验：指针必须落在本池 arena 内 */
    unsigned char *p = (unsigned char *)b;
    if (p < pool->arena || p >= pool->arena + pool->capacity) {
        fprintf(stderr, "pool_free: 指针不属于本内存池\n");
        return;
    }
    if (b->is_free) {
        fprintf(stderr, "pool_free: 重复释放\n");
        return;
    }

    freelist_push(pool, b);
    block_coalesce(pool, b);
}

/* =============================================================================
 * 调试打印：看清物理块 + 空闲链
 * ============================================================================= */

static void pool_dump(const MemPool *pool, const char *title)
{
    printf("\n==== %s ====\n", title);
    printf("物理布局（地址从低到高）：\n");

    int i = 0;
    for (Block *b = pool->phys_head; b; b = b->phys_next, i++) {
        printf("  [#%d] header=%p  payload=%p  size=%4zu  %s\n", i, (void *)b,
               user_from_block(b), b->size, b->is_free ? "FREE" : "BUSY");
    }

    printf("空闲链表：");
    if (!pool->free_head) {
        printf(" (空)\n");
    } else {
        printf("\n");
        for (Block *f = pool->free_head; f; f = f->free_next) {
            printf("  -> %p (size=%zu)\n", (void *)f, f->size);
        }
    }
}

/* =============================================================================
 * 演示：分配、释放、观察分裂与合并
 * ============================================================================= */

static void demo(void)
{
    MemPool pool;
    if (!pool_init(&pool, POOL_BYTES)) {
        fprintf(stderr, "pool_init 失败\n");
        exit(1);
    }

    pool_dump(&pool, "初始：一整块空闲");

    /* 连续分配三块，逼出分裂 */
    void *a = pool_alloc(&pool, 100);
    void *b = pool_alloc(&pool, 200);
    void *c = pool_alloc(&pool, 80);
    printf("\nalloc: a=%p (100), b=%p (200), c=%p (80)\n", a, b, c);
    pool_dump(&pool, "分配 a/b/c 之后（大块被分裂）");

    /* 释放中间的 b：两边若忙，则中间出现空洞（碎片） */
    pool_free(&pool, b);
    b = NULL;
    pool_dump(&pool, "释放 b 之后（中间空洞）");

    /* 再释放 a：应与右边空闲（原 b）合并 */
    pool_free(&pool, a);
    a = NULL;
    pool_dump(&pool, "释放 a 之后（应与原 b 区域合并）");

    /* 释放 c：理想情况整池又并回一大块 */
    pool_free(&pool, c);
    c = NULL;
    pool_dump(&pool, "释放 c 之后（期望重新合并成大块）");

    /* 再分配一块较大的，验证合并是否成功 */
    void *d = pool_alloc(&pool, 500);
    printf("\n再 alloc 500 -> %s\n", d ? "成功" : "失败");
    if (d) {
        memset(d, 0xAB, 500); /* 真能写，说明用户区有效 */
        pool_dump(&pool, "分配 500 之后");
        pool_free(&pool, d);
    }

    pool_destroy(&pool);
    puts("\n演示结束。对照上面 FREE/BUSY 看分裂与合并。");
}

/*
 * 交互小菜单：自己动手 alloc/free 观察
 * 用「句柄号」记住指针，避免手打地址。
 */
#define MAX_HANDLES 16

static void interactive(void)
{
    MemPool pool;
    void *handles[MAX_HANDLES];
    size_t sizes[MAX_HANDLES];

    if (!pool_init(&pool, POOL_BYTES)) {
        fprintf(stderr, "pool_init 失败\n");
        exit(1);
    }
    for (int i = 0; i < MAX_HANDLES; i++) {
        handles[i] = NULL;
        sizes[i] = 0;
    }

    puts("命令: a <字节> | f <句柄> | d | q");
    puts("  a 128   —— 分配，返回句柄号");
    puts("  f 0     —— 释放句柄 0");
    puts("  d       —— dump 池状态");
    puts("  q       —— 退出");

    char line[128];
    while (printf("> "), fgets(line, sizeof line, stdin)) {
        char cmd;
        unsigned long arg;
        if (sscanf(line, " %c %lu", &cmd, &arg) >= 1) {
            if (cmd == 'q') {
                break;
            }
            if (cmd == 'd') {
                pool_dump(&pool, "当前状态");
                continue;
            }
            if (cmd == 'a') {
                if (sscanf(line, " %c %lu", &cmd, &arg) != 2) {
                    puts("用法: a <size>");
                    continue;
                }
                int slot = -1;
                for (int i = 0; i < MAX_HANDLES; i++) {
                    if (!handles[i]) {
                        slot = i;
                        break;
                    }
                }
                if (slot < 0) {
                    puts("句柄用完了，先 free 一些");
                    continue;
                }
                void *p = pool_alloc(&pool, (size_t)arg);
                if (!p) {
                    printf("分配 %lu 失败（空闲块不够大）\n", arg);
                } else {
                    handles[slot] = p;
                    sizes[slot] = (size_t)arg;
                    printf("句柄 %d -> %p (size=%lu)\n", slot, p, arg);
                }
                continue;
            }
            if (cmd == 'f') {
                if (sscanf(line, " %c %lu", &cmd, &arg) != 2 || arg >= MAX_HANDLES) {
                    puts("用法: f <handle>");
                    continue;
                }
                if (!handles[arg]) {
                    puts("该句柄空闲");
                    continue;
                }
                pool_free(&pool, handles[arg]);
                printf("已释放句柄 %lu\n", arg);
                handles[arg] = NULL;
                sizes[arg] = 0;
                continue;
            }
        }
        puts("未知命令");
    }

    for (int i = 0; i < MAX_HANDLES; i++) {
        if (handles[i]) {
            pool_free(&pool, handles[i]);
        }
    }
    pool_destroy(&pool);
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "-i") == 0) {
        interactive();
    } else {
        demo();
        puts("\n提示：./mempool -i 可进入交互模式自己 alloc/free。");
    }
    return 0;
}
