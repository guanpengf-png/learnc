/*
 * =============================================================================
 * 教学版终端贪吃蛇 —— 刻意练习：结构体 + 指针
 * =============================================================================
 *
 * 覆盖的 C 基础：
 *   - struct / typedef
 *   - 指针传参（在函数里改调用者的数据）
 *   - 指针访问结构体成员：p->x
 *   - 动态数组：malloc / realloc / free
 *   - 数组名退化为指针、指针算术
 *   - 一维缓冲模拟二维地图
 *   - const 指针（只读遍历）
 *   - 二级指针（扩容时改「指针变量本身」）
 *   - 函数指针（按键表）
 *
 * 操作：wasd / hjkl 移动，单独回车=按原方向走一格，q 退出
 * 模式：回合制（每按一次键走一格），方便盯着指针状态调试
 */

#include <stdio.h>   /* printf / putchar / getchar / fwrite */
#include <stdlib.h>  /* malloc / realloc / free / rand / exit */
#include <string.h>  /* memset */
#include <time.h>    /* time —— 给 srand 当种子 */
#include <ctype.h>   /* tolower —— 把大写键转成小写统一处理 */

/* 地图宽度（列数）、高度（行数） */
#define MAP_W 24
#define MAP_H 14
/* 蛇身动态数组的初始容量（还没分配时，第一次扩容用这个起步） */
#define INIT_CAP 16

/* =============================================================================
 * 结构体定义
 * ============================================================================= */

/*
 * Point：平面上的一个格子坐标。
 * 既表示蛇身上某一节的位置，也表示方向向量（例如 {1,0} 表示向右）。
 */
typedef struct {
    int x; /* 列，从左到右，0 .. MAP_W-1 */
    int y; /* 行，从上到下，0 .. MAP_H-1 */
} Point;

/*
 * Snake：整条蛇的状态。
 * body 是「动态数组」：堆上连续的 Point，body[0] 永远是蛇头。
 */
typedef struct {
    Point *body; /* 指向堆上 Point 数组的指针；NULL 表示还没分配 */
    int len;     /* 当前有多少节（有效元素个数） */
    int cap;     /* 堆上实际能装多少节（容量 >= len） */
    Point dir;   /* 当前前进方向，例如 {0,-1} 向上 */
} Snake;

/*
 * Game：一局游戏的全部状态。
 * 把蛇、食物、地图、分数捆在一起，后面函数都接收 Game *，一次改全套。
 */
typedef struct {
    Snake snake; /* 蛇（内嵌结构体，不是指针） */
    Point food;  /* 食物坐标 */
    char *cells; /* 地图像素缓冲：一维 char 数组，长度 MAP_W * MAP_H */
    int score;   /* 得分 */
    int alive;   /* 1=还活着，0=撞墙/撞自己结束 */
} Game;

/* =============================================================================
 * 小工具函数（坐标、比较、边界）
 * ============================================================================= */

/*
 * idx —— 把二维坐标 (x,y) 换成一维下标。
 *
 * 干什么：
 *   地图在内存里是一行接一行排开的：
 *     第 0 行占下标 0 .. MAP_W-1
 *     第 1 行占下标 MAP_W .. 2*MAP_W-1
 *   所以格子 (x,y) 的下标 = y * 行宽 + x。
 *
 * 参数：x 列，y 行
 * 返回：对应的一维下标（整数）
 */
static int idx(int x, int y)
{
    return y * MAP_W + x;
}

/*
 * cell_at —— 拿到地图上某个格子的「指针」。
 *
 * 干什么：
 *   返回指向 g->cells 里那个字节的地址。
 *   调用者可以写： *cell_at(g, x, y) = '@';
 *   这是「通过指针改内存」的典型用法。
 *
 * 参数：
 *   g —— 游戏对象指针（要用 g->cells）
 *   x, y —— 格子坐标
 * 返回：
 *   char * —— 指向该格子字符的指针
 *
 * 指针点：
 *   g->cells 是首地址；加上偏移 idx 就是目标地址（指针算术）。
 */
static char *cell_at(Game *g, int x, int y)
{
    return g->cells + idx(x, y);
}

/*
 * point_eq —— 判断两个点是否同一格。
 *
 * 干什么：比较 a、b 的 x/y 是否都相等。
 *
 * 参数：
 *   a, b —— 指向 Point 的只读指针（const：函数内不许改 *a / *b）
 * 返回：
 *   1 表示相同，0 表示不同
 *
 * 为什么传指针：
 *   结构体用指针传，避免拷贝；const 表示「只读」。
 */
static int point_eq(const Point *a, const Point *b)
{
    return a->x == b->x && a->y == b->y;
}

/*
 * in_bounds —— 判断点是否还在地图里面。
 *
 * 干什么：检查 p 的坐标是否落在 [0, MAP_W) × [0, MAP_H)。
 * 撞墙判定会用到。
 *
 * 参数：p —— 要检查的点
 * 返回：1=在界内，0=出界
 */
static int in_bounds(const Point *p)
{
    return p->x >= 0 && p->x < MAP_W && p->y >= 0 && p->y < MAP_H;
}

/* =============================================================================
 * 蛇身动态数组：扩容 / 初始化 / 释放 / 撞自己检测
 * ============================================================================= */

/*
 * snake_reserve —— 保证蛇身数组至少能装 need 节。
 *
 * 干什么：
 *   如果当前容量 *cap 已经够用，什么也不做，返回成功。
 *   不够就 realloc 扩容（通常按 2 倍涨），并把新地址写回调用者。
 *
 * 参数（重点看二级指针）：
 *   body —— Point **，指向「Snake 里那个 body 指针变量」的地址
 *           因为 realloc 成功后地址可能变，必须 *body = 新地址
 *   cap  —— int *，指向容量变量，扩容后要同步改掉
 *   need —— 至少需要多少节
 *
 * 返回：
 *   1 成功，0 失败（内存不够）
 *
 * 教学要点：
 *   若写成 snake_reserve(Point *body, ...)，函数里 body = realloc(...)
 *   只改了「形参副本」，调用者 Snake.body 仍是旧地址 → 悬空/泄漏。
 *   所以必须 Point **，才能改调用者手里的那个指针。
 */
static int snake_reserve(Point **body, int *cap, int need)
{
    /* 容量够：不用扩 */
    if (need <= *cap) {
        return 1;
    }

    /* 算出新容量：没有就从 INIT_CAP 起，否则翻倍，直到 >= need */
    int ncap = *cap ? *cap * 2 : INIT_CAP;
    while (ncap < need) {
        ncap *= 2;
    }

    /*
     * realloc(*body, 字节数)：
     *   *body 是旧数组首地址；
     *   返回可能仍是旧地址，也可能是新地址（旧块会自动处理）。
     * sizeof *p 等价于 sizeof(Point)，跟指针类型走，改类型不易错。
     */
    Point *p = realloc(*body, (size_t)ncap * sizeof *p);
    if (!p) {
        return 0; /* 分配失败，旧 *body 仍然有效，别乱改 */
    }

    *body = p;   /* 写回：调用者的 body 指针换成新地址 */
    *cap = ncap; /* 写回：容量更新 */
    return 1;
}

/*
 * snake_init —— 创建一条初始长度为 3 的蛇。
 *
 * 干什么：
 *   1. 清空字段
 *   2. 分配动态数组
 *   3. 放好头和两节身体（水平排在 (x,y)、(x-1,y)、(x-2,y)）
 *   4. 初始方向设为向右 {1, 0}
 *
 * 参数：
 *   s —— 要初始化的 Snake（传指针才能改调用者的结构体）
 *   x, y —— 蛇头起始坐标
 */
static void snake_init(Snake *s, int x, int y)
{
    s->body = NULL; /* 还没分配，必须是 NULL，realloc/malloc 才安全 */
    s->len = 0;
    s->cap = 0;
    s->dir = (Point){1, 0}; /* 复合字面量：向右 */

    /* &s->body 取的是「结构体里 body 这个指针成员」的地址 → Point ** */
    if (!snake_reserve(&s->body, &s->cap, 4)) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    /* body[0] 头，body[1]、body[2] 身子，在头左边两格 */
    s->body[0] = (Point){x, y};
    s->body[1] = (Point){x - 1, y};
    s->body[2] = (Point){x - 2, y};
    s->len = 3;
}

/*
 * snake_free —— 释放蛇身占用的堆内存。
 *
 * 干什么：
 *   free 掉 body 数组，并把指针置 NULL、长度清零，避免悬空指针误用。
 *
 * 参数：s —— 蛇的指针
 */
static void snake_free(Snake *s)
{
    free(s->body);     /* free(NULL) 在标准里是安全的，什么也不做 */
    s->body = NULL;    /* 养成习惯：释放后立刻置空 */
    s->len = 0;
    s->cap = 0;
}

/*
 * snake_hits_self —— 判断「新头位置」会不会咬到自己身体。
 *
 * 干什么：
 *   从身体第二节开始，用指针依次走访每一节，看有没有和 head 重合。
 *   （第一节是旧头，移动后会变成脖子，新头一般不和「即将挪走的旧头」比。）
 *
 * 参数：
 *   s    —— 只读蛇状态
 *   head —— 打算占用的新头坐标
 * 返回：
 *   1=撞到自己，0=没有
 *
 * 指针点：
 *   p 从 body+1 走到 body+len（左闭右开），p++ 每次前进一个 Point。
 */
static int snake_hits_self(const Snake *s, const Point *head)
{
    const Point *p = s->body + 1;          /* 第二节 */
    const Point *end = s->body + s->len; /* 尾后指针：指向「最后一个的下一个」 */

    for (; p < end; p++) {
        if (point_eq(p, head)) {
            return 1;
        }
    }
    return 0;
}

/* =============================================================================
 * 食物
 * ============================================================================= */

/*
 * occupied_by_snake —— 某个格子是不是已经被蛇占着。
 *
 * 干什么：刷食物时要避开蛇身；整条 body 扫一遍。
 *
 * 参数：s 蛇，pt 候选坐标
 * 返回：1=被占用，0=空闲
 */
static int occupied_by_snake(const Snake *s, const Point *pt)
{
    const Point *p = s->body;
    const Point *end = s->body + s->len;

    for (; p < end; p++) {
        if (point_eq(p, pt)) {
            return 1;
        }
    }
    return 0;
}

/*
 * spawn_food —— 在空格子上随机放一颗食物。
 *
 * 干什么：
 *   反复随机 (x,y)，直到不和蛇重叠，写进 g->food。
 *   guard 防止极端情况下死循环（地图几乎被蛇填满时）。
 *
 * 参数：g —— 游戏指针（会改 g->food）
 */
static void spawn_food(Game *g)
{
    Point cand; /* 候选食物坐标（局部变量，在栈上） */
    int guard = 0;

    do {
        cand.x = rand() % MAP_W; /* 0 .. MAP_W-1 */
        cand.y = rand() % MAP_H;
        if (++guard > 10000) {
            break; /* 保底退出，避免卡死 */
        }
        /* &g->snake、&cand：取地址传给「接收指针」的函数 */
    } while (occupied_by_snake(&g->snake, &cand));

    g->food = cand; /* 结构体赋值：整份拷贝 x、y */
}

/* =============================================================================
 * 地图：清空、把蛇/食物画进缓冲、打印到终端
 * ============================================================================= */

/*
 * clear_cells —— 把整张地图缓冲刷成空格。
 *
 * 干什么：每一帧重画前，先清空旧痕迹。
 * memset(首地址, 填什么字节, 多少字节)：
 *   g->cells 是 char *，也就是数组首地址。
 */
static void clear_cells(Game *g)
{
    memset(g->cells, ' ', (size_t)MAP_W * MAP_H);
}

/*
 * paint —— 根据当前逻辑状态，把符号写进 cells 缓冲。
 *
 * 干什么（不负责打印，只负责「画到内存」）：
 *   1. 清空
 *   2. 食物写成 '*'
 *   3. 蛇头写成 '@'，其余身体写成 'o'
 *
 * 参数：g —— 会改 g->cells 的内容
 *
 * 指针点：
 *   cell_at 返回 char *，前面加 * 就是写那个字节。
 *   用 p 从 body 扫到 end，比下标 i++ 更直观地练指针遍历。
 */
static void paint(Game *g)
{
    clear_cells(g);

    /* 食物：解引用 cell 指针，写入字符 */
    *cell_at(g, g->food.x, g->food.y) = '*';

    Point *p = g->snake.body;
    Point *end = g->snake.body + g->snake.len;

    if (p < end) {
        *cell_at(g, p->x, p->y) = '@'; /* 头 */
        /* 从第二节开始画身体 */
        for (p = g->snake.body + 1; p < end; p++) {
            *cell_at(g, p->x, p->y) = 'o';
        }
    }
}

/*
 * draw —— 把 cells 缓冲打印成带边框的画面。
 *
 * 干什么：
 *   清屏 → 打分数 → 打上边框 → 逐行输出地图 → 下边框。
 *   若已死亡，再打印 Game Over。
 *
 * 参数：
 *   g —— const Game *：只读，不允许在 draw 里改游戏状态
 *
 * 指针点：
 *   row 起初指向第 0 行首；每打完一行 row += MAP_W 跳到下一行。
 */
static void draw(const Game *g)
{
    /* \033[2J 清屏，\033[H 光标回到左上角（常见终端 ANSI 转义） */
    printf("\033[2J\033[H");
    printf("score=%d  len=%d  wasd移动 q退出\n", g->score, g->snake.len);

    /* 上边框 +----+ */
    printf("+");
    for (int i = 0; i < MAP_W; i++) {
        putchar('-');
    }
    printf("+\n");

    const char *row = g->cells; /* 指向当前要打印的那一行的第一个字符 */
    for (int y = 0; y < MAP_H; y++) {
        putchar('|');
        /* 一次写出连续 MAP_W 个字节，避免一个个 putchar */
        fwrite(row, 1, MAP_W, stdout);
        putchar('|');
        putchar('\n');
        row += MAP_W; /* 指针前进一整行 */
    }

    /* 下边框 */
    printf("+");
    for (int i = 0; i < MAP_W; i++) {
        putchar('-');
    }
    printf("+\n");

    if (!g->alive) {
        printf("Game Over! 最终得分 %d\n", g->score);
    }
}

/* =============================================================================
 * 移动逻辑（核心）
 * ============================================================================= */

/*
 * snake_step —— 蛇朝当前方向走一步。
 *
 * 干什么：
 *   计算新头 = 旧头 + dir。
 *   身体整体「往后挪」：原来的头变成第二节，第二节变第三节……
 *   - grow == 0：长度不变，等于尾巴自然离开最后一格
 *   - grow == 1：长度 +1，尾巴多留一节（吃到食物）
 *
 * 参数：
 *   s    —— 蛇（会被改 body / len）
 *   grow —— 是否变长
 *
 * 注意：
 *   必须从尾巴往头拷贝（i 从大到小），否则会覆盖还没用的旧数据。
 */
static void snake_step(Snake *s, int grow)
{
    Point new_head = {
        s->body[0].x + s->dir.x,
        s->body[0].y + s->dir.y,
    };

    if (grow) {
        /* 变长：先确保容量够 len+1 */
        if (!snake_reserve(&s->body, &s->cap, s->len + 1)) {
            fprintf(stderr, "realloc failed\n");
            exit(1);
        }
        /* 每一节向后移一格：body[i] = body[i-1] */
        for (int i = s->len; i > 0; i--) {
            s->body[i] = s->body[i - 1];
        }
        s->len++;           /* 有效长度 +1 */
        s->body[0] = new_head; /* 新头放最前 */
    } else {
        /* 不变长：同样后移，最后一节被覆盖丢掉 */
        for (int i = s->len - 1; i > 0; i--) {
            s->body[i] = s->body[i - 1];
        }
        s->body[0] = new_head;
    }
}

/*
 * game_tick —— 推进一帧游戏逻辑（尝试走一步）。
 *
 * 干什么：
 *   1. 已死亡则直接返回
 *   2. 预计算下一步坐标 next
 *   3. 出界或撞自己 → alive=0
 *   4. 否则 snake_step；若踩到食物则加分并刷新食物
 *
 * 参数：g —— 整局游戏（分数、蛇、食物都可能变）
 */
static void game_tick(Game *g)
{
    if (!g->alive) {
        return;
    }

    /* 下一步头会到哪：读 body[0] 和 dir（都通过 g->snake 访问） */
    Point next = {
        g->snake.body[0].x + g->snake.dir.x,
        g->snake.body[0].y + g->snake.dir.y,
    };

    /* &next：把局部变量的地址传出去，让函数用指针读它 */
    if (!in_bounds(&next) || snake_hits_self(&g->snake, &next)) {
        g->alive = 0;
        return;
    }

    /* 新头是否正好在食物上？ */
    int grow = point_eq(&next, &g->food);
    snake_step(&g->snake, grow);
    if (grow) {
        g->score += 10;
        spawn_food(g);
    }
}

/* =============================================================================
 * 输入：用函数指针做方向键分发
 * ============================================================================= */

/*
 * DirHandler —— 函数指针类型。
 *
 * 含义：指向「参数是 Snake *、返回 void」的函数。
 * 例如 go_up、go_down 都符合这个签名，可以存进一张表里统一调用。
 */
typedef void (*DirHandler)(Snake *s);

/*
 * go_up —— 把方向改成向上，但不能「立刻掉头」。
 *
 * 干什么：若当前不是正在向下，则 dir = {0, -1}。
 * 禁止向下时直接按上，否则蛇会一头扎进自己第二节。
 */
static void go_up(Snake *s)
{
    if (s->dir.y != 1) { /* 当前不是向下才能改成向上 */
        s->dir = (Point){0, -1};
    }
}

/* go_down —— 改成向下 {0, 1}，若正在向上则忽略 */
static void go_down(Snake *s)
{
    if (s->dir.y != -1) {
        s->dir = (Point){0, 1};
    }
}

/* go_left —— 改成向左 {-1, 0}，若正在向右则忽略 */
static void go_left(Snake *s)
{
    if (s->dir.x != 1) {
        s->dir = (Point){-1, 0};
    }
}

/* go_right —— 改成向右 {1, 0}，若正在向左则忽略 */
static void go_right(Snake *s)
{
    if (s->dir.x != -1) {
        s->dir = (Point){1, 0};
    }
}

/*
 * KeyBind —— 「按键字符 → 处理函数」的一条绑定。
 * fn 的类型是函数指针 DirHandler。
 */
typedef struct {
    char key;      /* 键盘字符，如 'w' */
    DirHandler fn; /* 对应动作函数的地址 */
} KeyBind;

/*
 * BINDS —— 静态按键表（存在静态区，程序启动时就有）。
 * 同一方向可以绑多个键（wasd 和 vim 风 hjkl）。
 */
static const KeyBind BINDS[] = {
    {'w', go_up},
    {'k', go_up},
    {'s', go_down},
    {'j', go_down},
    {'a', go_left},
    {'h', go_left},
    {'d', go_right},
    {'l', go_right},
};

/*
 * handle_key —— 处理一个输入字符。
 *
 * 干什么：
 *   - 'q'：返回 0，让主循环退出
 *   - 若在 BINDS 里找到对应键：通过函数指针 b->fn(&g->snake) 转向
 *   - 然后无论是否识别到方向键，都 game_tick 走一步（未知键也走，保持节奏）
 *
 * 参数：g 游戏，ch 字符（可能是大写，会 tolower）
 * 返回：1 继续游戏，0 玩家要求退出
 *
 * 指针点：
 *   b、end 用指针遍历数组；b->fn(...) 是「调用函数指针」。
 */
static int handle_key(Game *g, int ch)
{
    ch = tolower(ch);
    if (ch == 'q') {
        return 0;
    }

    const KeyBind *b = BINDS;
    const KeyBind *end = BINDS + sizeof BINDS / sizeof *BINDS;
    /* sizeof BINDS / sizeof *BINDS = 元素个数 */

    for (; b < end; b++) {
        if (b->key == ch) {
            b->fn(&g->snake); /* 等价于直接调用 go_up(&g->snake) 等 */
            break;
        }
    }

    game_tick(g);
    return 1;
}

/* =============================================================================
 * 游戏生命周期：创建 / 销毁
 * ============================================================================= */

/*
 * game_init —— 开局：分配地图、造蛇、刷食物、分数清零。
 *
 * 干什么：
 *   malloc 一块 MAP_W*MAP_H 的 char 当画布；
 *   蛇出生在地图中央；alive=1。
 *
 * 参数：g —— 通常是 main 里的局部 Game，取其地址传入
 */
static void game_init(Game *g)
{
    g->cells = malloc((size_t)MAP_W * MAP_H);
    if (!g->cells) {
        fprintf(stderr, "malloc cells failed\n");
        exit(1);
    }

    snake_init(&g->snake, MAP_W / 2, MAP_H / 2);
    g->score = 0;
    g->alive = 1;
    spawn_food(g);
}

/*
 * game_free —— 收尾：释放蛇身和地图，避免内存泄漏。
 *
 * 干什么：先 free 蛇，再 free cells，并把 cells 置 NULL。
 */
static void game_free(Game *g)
{
    snake_free(&g->snake);
    free(g->cells);
    g->cells = NULL;
}

/* =============================================================================
 * 程序入口
 * ============================================================================= */

/*
 * main —— 程序从这里开始。
 *
 * 干什么：
 *   1. 用当前时间初始化随机数种子（食物位置才随机）
 *   2. 初始化游戏
 *   3. 循环：画图 → 读一行输入 → 处理按键/回车
 *   4. 结束后再画最后一帧，释放内存
 *
 * 输入约定：
 *   每次先敲一个字符再回车（或只敲回车=按原方向走）。
 *   这是回合制，方便你单步对照 body 数组。
 */
int main(void)
{
    /* 没有 srand 的话，每次 rand() 序列都一样，食物位置会固定 */
    srand((unsigned)time(NULL));

    Game game;        /* 栈上的 Game 对象 */
    game_init(&game); /* 传入地址，让 init 填写它的字段 */

    puts("教学贪吃蛇：每次输入一个字符后蛇走一步。");
    puts("w/a/s/d（或 h/j/k/l）转向，回车也可前进，q 退出。");
    printf("按回车开始...");
    (void)getchar(); /* (void) 表示故意忽略返回值，避免编译器告警 */

    while (game.alive) {
        paint(&game); /* 逻辑 → 内存画布 */
        draw(&game);  /* 内存画布 → 屏幕 */
        printf("> ");
        fflush(stdout); /* 立刻把提示符刷出来（有的终端会缓冲） */

        int ch = getchar();
        if (ch == EOF) { /* Ctrl+D 或输入结束 */
            break;
        }

        if (ch == '\n') {
            /* 用户只按了回车：不转向，按当前 dir 走一步 */
            game_tick(&game);
            continue;
        }

        /*
         * 用户通常输入：字符 + 换行。
         * 第一个 getchar 拿到字符后，还要消化掉后面的 '\n'，
         * 否则下一次循环会把 '\n' 当成「空回车走一步」。
         */
        int rest = getchar();
        if (rest != '\n' && rest != EOF) {
            /* 一次多敲了几个字符：丢弃到行尾，简单处理即可 */
            while (rest != '\n' && rest != EOF) {
                rest = getchar();
            }
        }

        if (!handle_key(&game, ch)) {
            break; /* 按了 q */
        }
    }

    /* 死亡或退出后，再显示最终画面 */
    paint(&game);
    draw(&game);
    game_free(&game);
    return 0;
}
