/*
 * =============================================================================
 * 电商类目树（纯递归练习版）
 * =============================================================================
 *
 * 类目是多叉树。本文件用「左孩子 - 右兄弟」表示：
 *
 *   first_child  → 第一个子类目
 *   next_sibling → 同级下一个类目
 *
 * 练习重点：所有「走遍树」的操作都用递归，不用 while 扫整棵树。
 *   - 往下钻：递归 first_child
 *   - 同级挪：递归 next_sibling
 *
 * 编译运行：
 *   cd day8/category_tree && make && ./category
 *   ./category -i          # 交互练习
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 64

/* -----------------------------------------------------------------------------
 * 一个类目节点
 *
 * product_count：挂在「这一级自己」上的商品数量（不含子孙）
 * 统计整棵子树时，要用递归把子孙的也加起来
 * ----------------------------------------------------------------------------- */
typedef struct Category {
    char name[NAME_LEN];
    int product_count;
    struct Category *first_child; /* 第一个子类目 */
    struct Category *next_sibling; /* 右边的兄弟类目 */
} Category;

/* =============================================================================
 * 创建 / 挂接（建树用）
 * ============================================================================= */

/*
 * category_new —— 新建一个类目节点（还没挂到树上）
 */
static Category *category_new(const char *name, int product_count)
{
    Category *c = (Category *)calloc(1, sizeof(Category));
    if (!c) {
        return NULL;
    }
    snprintf(c->name, NAME_LEN, "%s", name);
    c->product_count = product_count;
    c->first_child = NULL;
    c->next_sibling = NULL;
    return c;
}

/*
 * category_add_child —— 把 child 挂到 parent 的孩子列表最前面
 * （建树辅助；真正「遍历/查找/统计」全在后面的递归函数里）
 */
static void category_add_child(Category *parent, Category *child)
{
    if (!parent || !child) {
        return;
    }
    child->next_sibling = parent->first_child;
    parent->first_child = child;
}

/* =============================================================================
 * 递归 1：缩进打印整棵树
 *
 * 大白话：
 *   1. 先打印「我自己」
 *   2. 再递归打印「我的孩子们」（更深一层缩进）
 *   3. 再递归打印「我的兄弟们」（同一层缩进）
 * ============================================================================= */

static void category_print(const Category *c, int depth)
{
    if (c == NULL) {
        return; /* 递归出口：空节点啥也不干 */
    }

    /* 缩进：深度几层就打几个 "  " */
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("- %s（本级商品 %d）\n", c->name, c->product_count);

    /* 往下：处理所有子类目 */
    category_print(c->first_child, depth + 1);

    /* 同级：处理右边的兄弟（注意 depth 不变） */
    category_print(c->next_sibling, depth);
}

/* =============================================================================
 * 递归 2：按名字查找
 *
 * 顺序：先看自己 → 再搜子树 → 再搜兄弟
 * 找到就返回指针；整棵都没有则返回 NULL
 * ============================================================================= */

static Category *category_find(Category *c, const char *name)
{
    if (c == NULL) {
        return NULL;
    }
    if (strcmp(c->name, name) == 0) {
        return c; /* 找到自己 */
    }

    /* 先在孩子里找 */
    Category *in_child = category_find(c->first_child, name);
    if (in_child != NULL) {
        return in_child;
    }

    /* 孩子没有，再问兄弟 */
    return category_find(c->next_sibling, name);
}

/* =============================================================================
 * 递归 3：统计「某节点为根的子树」里商品总数
 *
 * 公式：
 *   总数 = 我自己的 product_count
 *        + 第一个孩子整棵子树的总数
 *        + （注意）兄弟不属于「我的子树」，统计子树时不要加兄弟！
 *
 * 所以这里只递归 first_child 一条线；
 * 但孩子还有兄弟，需要一个「只沿着 sibling 累加各孩子子树」的助手。
 * ============================================================================= */

/* 累加「同一层的一串兄弟」各自子树的商品数 —— 自身也是递归 */
static int category_count_forest(const Category *c);

static int category_count_subtree(const Category *c)
{
    if (c == NULL) {
        return 0;
    }
    /* 自己的 + 所有孩子（孩子之间是兄弟，用 forest 累加） */
    return c->product_count + category_count_forest(c->first_child);
}

static int category_count_forest(const Category *c)
{
    if (c == NULL) {
        return 0;
    }
    /* 当前这个孩子的整棵子树 + 右边下一个兄弟 */
    return category_count_subtree(c) + category_count_forest(c->next_sibling);
}

/* =============================================================================
 * 递归 4：树高（根到最深叶子的边数；空树为 -1，单节点为 0）
 * ============================================================================= */

static int max_int(int a, int b)
{
    return a > b ? a : b;
}

/* 一串兄弟里的最大高度 */
static int category_height_forest(const Category *c);

static int category_height(const Category *c)
{
    if (c == NULL) {
        return -1;
    }
    /* 高度 = 1 + 孩子们里最高的那个（没孩子则为 0） */
    return 1 + category_height_forest(c->first_child);
}

static int category_height_forest(const Category *c)
{
    if (c == NULL) {
        return -1; /* 没有孩子时，上面 1+(-1)=0，即叶子高度 0 */
    }
    int h_here = category_height(c);
    int h_rest = category_height_forest(c->next_sibling);
    return max_int(h_here, h_rest);
}

/* =============================================================================
 * 递归 5：面包屑路径（全部商品 > 电子产品 > 手机 > 苹果）
 *
 * 用「路径栈 + 递归」：
 *   走到节点 c 时，把 c->name 放进 stack[depth]
 *   - 名字对上：成功，记下长度
 *   - 否则先递归孩子（depth+1）
 *   - 再递归兄弟（depth 不变：兄弟替换当前格，共享同一父前缀）
 * ============================================================================= */

static int category_build_path(const Category *c, const char *target,
                               const char *stack[], int depth, int *out_len)
{
    if (c == NULL || depth >= 63) {
        return 0; /* 递归出口 */
    }

    stack[depth] = c->name;

    if (strcmp(c->name, target) == 0) {
        *out_len = depth + 1;
        return 1;
    }

    /* 往下钻：路径更长 */
    if (category_build_path(c->first_child, target, stack, depth + 1, out_len)) {
        return 1;
    }

    /* 同级兄弟：深度不变，盖掉当前这一格 */
    return category_build_path(c->next_sibling, target, stack, depth, out_len);
}

static void category_show_path(const Category *root, const char *target)
{
    const char *stack[64];
    int len = 0;

    if (!category_build_path(root, target, stack, 0, &len)) {
        printf("（找不到类目：%s）\n", target);
        return;
    }

    for (int i = 0; i < len; i++) {
        if (i > 0) {
            printf(" > ");
        }
        printf("%s", stack[i]);
    }
    printf("\n");
}

/* =============================================================================
 * 递归 6：列出所有叶子类目（没有子类目的节点）
 * ============================================================================= */

static void category_print_leaves(const Category *c)
{
    if (c == NULL) {
        return;
    }

    if (c->first_child == NULL) {
        printf("  · %s（商品 %d）\n", c->name, c->product_count);
    } else {
        category_print_leaves(c->first_child);
    }

    category_print_leaves(c->next_sibling);
}

/* =============================================================================
 * 递归 7：销毁整棵树（必须后序：先孩子、再兄弟、最后自己）
 * ============================================================================= */

static void category_free(Category *c)
{
    if (c == NULL) {
        return;
    }
    category_free(c->first_child);
    category_free(c->next_sibling);
    free(c);
}

/* =============================================================================
 * 造一棵示例电商类目树
 *
 * 全部商品
 * ├── 服装
 * │   ├── 女装（12）
 * │   └── 男装（8）
 * └── 电子产品
 *     ├── 电脑
 *     │   ├── 台式机（5）
 *     │   └── 笔记本（20）
 *     └── 手机
 *         ├── 安卓（30）
 *         └── 苹果（15）
 *
 * 注意 add_child 是头插，所以先加的会在后面；
 * 为让打印顺序好看，我们按「想要的逆序」添加。
 * ============================================================================= */

static Category *build_demo_tree(void)
{
    Category *root = category_new("全部商品", 0);

    Category *elec = category_new("电子产品", 0);
    Category *phone = category_new("手机", 0);
    Category *computer = category_new("电脑", 0);
    Category *apple = category_new("苹果", 15);
    Category *android = category_new("安卓", 30);
    Category *laptop = category_new("笔记本", 20);
    Category *desktop = category_new("台式机", 5);

    Category *cloth = category_new("服装", 0);
    Category *men = category_new("男装", 8);
    Category *women = category_new("女装", 12);

    /* 头插：后添加的显示在前面 */
    category_add_child(phone, android);
    category_add_child(phone, apple);

    category_add_child(computer, desktop);
    category_add_child(computer, laptop);

    category_add_child(elec, computer);
    category_add_child(elec, phone);

    category_add_child(cloth, men);
    category_add_child(cloth, women);

    category_add_child(root, cloth);
    category_add_child(root, elec);

    return root;
}

/* =============================================================================
 * 自动演示
 * ============================================================================= */

static void run_demo(void)
{
    Category *root = build_demo_tree();

    printf("==== 1. 递归打印类目树 ====\n");
    category_print(root, 0);

    printf("\n==== 2. 递归统计：整棵树商品总数 ====\n");
    printf("总数 = %d\n", category_count_subtree(root));

    printf("\n==== 3. 递归统计：只看「电子产品」子树 ====\n");
    {
        Category *elec = category_find(root, "电子产品");
        if (elec) {
            printf("电子产品子树商品数 = %d\n", category_count_subtree(elec));
        }
    }

    printf("\n==== 4. 递归求树高 ====\n");
    printf("高度 = %d（根到最深叶子的层数差）\n", category_height(root));

    printf("\n==== 5. 递归查找 + 面包屑路径 ====\n");
    printf("苹果：");
    category_show_path(root, "苹果");
    printf("女装：");
    category_show_path(root, "女装");
    printf("不存在：");
    category_show_path(root, "冰箱");

    printf("\n==== 6. 递归列出叶子类目 ====\n");
    category_print_leaves(root);

    printf("\n==== 7. 递归销毁树 ====\n");
    category_free(root);
    printf("已 free 全部节点。\n");

    printf("\n提示：./category -i 进入交互，自己找类目、看路径、看子树统计。\n");
}

/* =============================================================================
 * 交互：在演示树上练习递归接口
 * ============================================================================= */

static void print_help(void)
{
    printf("命令：\n");
    printf("  p           打印整棵类目树\n");
    printf("  f <名字>    查找类目是否存在\n");
    printf("  c <名字>    统计该类目子树商品数（不写名字=整棵树）\n");
    printf("  h           树高\n");
    printf("  path <名字> 打印面包屑路径\n");
    printf("  leaves      列出所有叶子类目\n");
    printf("  help        帮助\n");
    printf("  q           退出\n");
}

static void run_interactive(void)
{
    Category *root = build_demo_tree();
    char line[256];
    char cmd[32];
    char arg[NAME_LEN];

    printf("电商类目树 · 交互练习（底层全是递归）\n");
    print_help();

    while (1) {
        printf("\n> ");
        if (!fgets(line, sizeof line, stdin)) {
            break;
        }

        arg[0] = '\0';
        int n = sscanf(line, "%31s %63s", cmd, arg);
        if (n < 1) {
            continue;
        }

        if (strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0) {
            break;
        } else if (strcmp(cmd, "help") == 0) {
            print_help();
        } else if (strcmp(cmd, "p") == 0) {
            category_print(root, 0);
        } else if (strcmp(cmd, "h") == 0) {
            printf("高度 = %d\n", category_height(root));
        } else if (strcmp(cmd, "leaves") == 0) {
            category_print_leaves(root);
        } else if (strcmp(cmd, "f") == 0) {
            if (arg[0] == '\0') {
                printf("用法：f <名字>\n");
                continue;
            }
            Category *hit = category_find(root, arg);
            printf("%s\n", hit ? "找到了" : "没有这个类目");
        } else if (strcmp(cmd, "c") == 0) {
            if (arg[0] == '\0') {
                printf("整树商品数 = %d\n", category_count_subtree(root));
            } else {
                Category *hit = category_find(root, arg);
                if (!hit) {
                    printf("没有这个类目\n");
                } else {
                    printf("「%s」子树商品数 = %d\n", arg, category_count_subtree(hit));
                }
            }
        } else if (strcmp(cmd, "path") == 0) {
            if (arg[0] == '\0') {
                printf("用法：path <名字>\n");
                continue;
            }
            category_show_path(root, arg);
        } else {
            printf("未知命令。输入 help 查看。\n");
        }
    }

    category_free(root);
}

/* =============================================================================
 * 主函数
 * ============================================================================= */

int main(int argc, char **argv)
{
    if (argc >= 2 && strcmp(argv[1], "-i") == 0) {
        run_interactive();
    } else {
        run_demo();
    }
    return 0;
}
