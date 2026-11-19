/*
 * pet_manager.c —— 控制台版「宠物管理系统」
 *
 * 功能：
 *   1. 添加宠物   2. 查看全部   3. 查找   4. 修改
 *   5. 删除       6. 排序       7. 统计   8. 保存 / 0. 退出
 *
 * 知识点（都能在你那份 C 语言笔记里找到对应章节）：
 *   结构体 + 结构体数组      —— 第十二章 数据类型
 *   字符串处理 strtok/strstr —— 字符与字符串
 *   文件读写 fopen/fgets     —— 文件操作
 *   qsort 函数指针           —— 指针与函数
 *   输入校验 fgets + strtol  —— 避免 scanf 的缓冲区残留问题
 *
 * 编译：
 *   clang -Wall -Wextra pet_manager.c -o pet_manager
 * 运行：
 *   ./pet_manager
 *
 * 数据文件：当前目录下的 pets.txt（纯文本，可以直接用编辑器打开看）
 *   格式：编号|名字|种类|年龄|体重|主人     （# 开头的是注释行）
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------------- 常量 ---------------- */
#define MAX_PETS    100          /* 最多能存多少只 */
#define DATA_FILE   "pets.txt"   /* 数据文件名 */
#define NAME_LEN    32
#define SPECIES_LEN 16
#define OWNER_LEN   32

/* ---------------- 结构体 ---------------- */
typedef struct {
    int    id;                    /* 编号，自动生成 */
    char   name[NAME_LEN];        /* 名字 */
    char   species[SPECIES_LEN];  /* 种类：狗 / 猫 / 鸟 / 兔 ... */
    int    age;                   /* 年龄（岁） */
    double weight;                /* 体重（kg） */
    char   owner[OWNER_LEN];      /* 主人 */
} Pet;

/* ---------------- 全局数据 ---------------- */
static Pet g_pets[MAX_PETS];
static int g_count = 0;           /* 当前宠物数量 */

/* =========================================================
   0. 输入工具：统一从这里读，避免 scanf 的各种坑
   ========================================================= */

/* 前向声明：输入流意外结束时需要先把数据存盘 */
static void save_pets(void);

static int g_eof = 0;             /* 标准输入是否已结束（管道读完 / Ctrl-D） */

/* 输入结束时统一处理：保存后退出，避免在循环里空转 */
static void handle_eof(void)
{
    printf("\n输入已结束，自动保存并退出。\n");
    save_pets();
    exit(0);
}

/* 读一整行，去掉结尾的 \n；输入太长时丢弃本行剩余内容，防止污染下一次读取 */
static void read_line(char *buf, size_t size)
{
    if (size == 0) return;

    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        g_eof = 1;                /* EOF 或出错 */
        return;
    }

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';      /* 正常的短行：去掉换行符 */
        return;
    }

    /* 没有换行符说明超长了，把这一行多出来的字符吃掉 */
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* 丢弃 */
    }
}

/* 读一个整数，带范围校验；输入非法就重新问，直到合法为止 */
static int read_int(const char *prompt, int lo, int hi)
{
    char line[64];

    for (;;) {
        if (g_eof) handle_eof();
        printf("%s", prompt);
        read_line(line, sizeof line);
        if (g_eof) handle_eof();

        char *end = NULL;
        long v = strtol(line, &end, 10);

        /* end == line 表示一个字符都没读到；*end 非 \0 表示后面还有垃圾字符 */
        if (end == line || *end != '\0') {
            printf("  ✗ 输入无效，请输入数字\n");
            continue;
        }
        if (v < (long)lo || v > (long)hi) {
            printf("  ✗ 范围应为 %d ~ %d\n", lo, hi);
            continue;
        }
        return (int)v;
    }
}

/* 读一个浮点数，带下限校验（比如体重要求 > 0） */
static double read_double(const char *prompt, double lo, double hi)
{
    char line[64];

    for (;;) {
        if (g_eof) handle_eof();
        printf("%s", prompt);
        read_line(line, sizeof line);
        if (g_eof) handle_eof();

        char *end = NULL;
        double v = strtod(line, &end);

        if (end == line || *end != '\0') {
            printf("  ✗ 输入无效，请输入数字\n");
            continue;
        }
        if (v < lo || v > hi) {
            printf("  ✗ 范围应为 %.1f ~ %.1f\n", lo, hi);
            continue;
        }
        return v;
    }
}

/* 读一段文本；require 为 1 时不允许空输入 */
static void read_text(const char *prompt, char *buf, size_t size, int require)
{
    for (;;) {
        if (g_eof) handle_eof();
        printf("%s", prompt);
        read_line(buf, size);
        if (g_eof) handle_eof();

        if (*buf != '\0' || !require) {
            return;
        }
        printf("  ✗ 内容不能为空\n");
    }
}

/* 读 y / n */
static int read_yes_no(const char *prompt)
{
    char line[32];

    for (;;) {
        if (g_eof) handle_eof();
        printf("%s", prompt);
        read_line(line, sizeof line);
        if (g_eof) handle_eof();
        if (line[0] == 'y' || line[0] == 'Y') return 1;
        if (line[0] == 'n' || line[0] == 'N') return 0;
        printf("  ✗ 请输入 y 或 n\n");
    }
}

/* =========================================================
   1. 显示工具：让中文能对齐
   ========================================================= */

/*
 * 计算字符串在终端里占多少列。
 * UTF-8 里：ASCII 占 1 列；中文（3 字节）占 2 列；emoji（4 字节）占 2 列。
 * 不这样算，中文表格会歪。
 */
static int utf8_width(const char *s)
{
    size_t len = strlen(s);
    int    w = 0;
    size_t i = 0;

    while (i < len) {
        unsigned char c = (unsigned char)s[i];
        size_t step;

        if (c < 0x80) {              /* ASCII */
            w += 1;
            step = 1;
        } else if ((c >> 5) == 0x6) { /* 2 字节字符 */
            w += 1;
            step = 2;
        } else if ((c >> 4) == 0xE) { /* 3 字节，中文走这里 */
            w += 2;
            step = 3;
        } else {                      /* 4 字节，emoji */
            w += 2;
            step = 4;
        }
        i += (i + step <= len) ? step : 1;
    }
    return w;
}

/* 左对齐打印，不足 width 列就补空格 */
static void print_pad(const char *s, int width)
{
    printf("%s", s);
    int w = utf8_width(s);
    for (int i = w; i < width; i++) {
        putchar(' ');
    }
}

/* 表头 */
static void print_header(void)
{
    putchar('\n');
    print_pad("  编号", 8);
    print_pad("名字", 16);
    print_pad("种类", 10);
    print_pad("年龄", 8);
    print_pad("体重(kg)", 12);
    print_pad("主人", 12);
    putchar('\n');

    for (int i = 0; i < 66; i++) putchar('-');
    putchar('\n');
}

/* 打印一行宠物数据 */
static void print_pet(const Pet *p)
{
    char buf[32];

    snprintf(buf, sizeof buf, "  %d", p->id);
    print_pad(buf, 8);

    print_pad(p->name, 16);
    print_pad(p->species, 10);

    snprintf(buf, sizeof buf, "%d", p->age);
    print_pad(buf, 8);

    snprintf(buf, sizeof buf, "%.1f", p->weight);
    print_pad(buf, 12);

    print_pad(p->owner, 12);
    putchar('\n');
}

/* =========================================================
   2. 文件读写
   ========================================================= */

/* 从 pets.txt 载入数据；文件不存在就当作空表 */
static void load_pets(void)
{
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf("提示：未找到 %s，将从空数据开始。\n", DATA_FILE);
        return;
    }

    char line[256];
    int  line_no = 0;

    while (fgets(line, sizeof line, fp) != NULL) {
        line_no++;

        /* 去掉行尾换行 */
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        /* 跳过空行和注释行 */
        if (line[0] == '\0' || line[0] == '#') continue;

        if (g_count >= MAX_PETS) {
            printf("警告：数据超过 %d 条，后面的已忽略。\n", MAX_PETS);
            break;
        }

        /* 按 | 切成 6 段：编号|名字|种类|年龄|体重|主人 */
        char *fields[6];
        int   n = 0;
        char *p = line;

        while (n < 6) {
            fields[n++] = p;
            char *bar = strchr(p, '|');
            if (bar == NULL) break;
            *bar = '\0';
            p = bar + 1;
        }

        if (n != 6) {
            printf("警告：第 %d 行格式不正确，已跳过。\n", line_no);
            continue;
        }

        Pet pet;
        memset(&pet, 0, sizeof pet);

        pet.id     = atoi(fields[0]);
        pet.age    = atoi(fields[3]);
        pet.weight = atof(fields[4]);

        /* 用 snprintf 而不是 strcpy：保证不会写越界 */
        snprintf(pet.name,    sizeof pet.name,    "%s", fields[1]);
        snprintf(pet.species, sizeof pet.species, "%s", fields[2]);
        snprintf(pet.owner,   sizeof pet.owner,   "%s", fields[5]);

        g_pets[g_count++] = pet;
    }

    fclose(fp);
    printf("已载入 %d 条宠物记录。\n", g_count);
}

/* 保存到 pets.txt（覆盖写） */
static void save_pets(void)
{
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("✗ 无法写入 %s（检查当前目录是否有写权限）\n", DATA_FILE);
        return;
    }

    fprintf(fp, "# 编号|名字|种类|年龄|体重|主人\n");
    for (int i = 0; i < g_count; i++) {
        const Pet *p = &g_pets[i];
        fprintf(fp, "%d|%s|%s|%d|%.2f|%s\n",
                p->id, p->name, p->species, p->age, p->weight, p->owner);
    }

    fclose(fp);
    printf("✓ 已保存 %d 条记录到 %s\n", g_count, DATA_FILE);
}

/* =========================================================
   3. 查找辅助
   ========================================================= */

/* 按编号找下标，找不到返回 -1 */
static int find_by_id(int id)
{
    for (int i = 0; i < g_count; i++) {
        if (g_pets[i].id == id) return i;
    }
    return -1;
}

/* 生成下一个编号：当前最大编号 + 1 */
static int next_id(void)
{
    int max_id = 1000;                 /* 从 1001 开始编号 */
    for (int i = 0; i < g_count; i++) {
        if (g_pets[i].id > max_id) max_id = g_pets[i].id;
    }
    return max_id + 1;
}

/* =========================================================
   4. 各项功能
   ========================================================= */

static void add_pet(void)
{
    if (g_count >= MAX_PETS) {
        printf("✗ 已达上限 %d 条，请先删除一些记录。\n", MAX_PETS);
        return;
    }

    printf("\n--- 添加宠物 ---\n");

    Pet pet;
    memset(&pet, 0, sizeof pet);

    read_text("名字：", pet.name, sizeof pet.name, 1);
    read_text("种类（狗/猫/鸟/兔…）：", pet.species, sizeof pet.species, 1);
    pet.age    = read_int("年龄（0-100）：", 0, 100);
    pet.weight = read_double("体重 kg（0.1-200）：", 0.1, 200.0);
    read_text("主人：", pet.owner, sizeof pet.owner, 1);

    pet.id = next_id();
    g_pets[g_count++] = pet;

    printf("✓ 已添加：编号 %d，%s\n", pet.id, pet.name);
}

static void list_pets(void)
{
    if (g_count == 0) {
        printf("\n（暂无宠物记录，先按 1 添加吧）\n");
        return;
    }

    print_header();
    for (int i = 0; i < g_count; i++) {
        print_pet(&g_pets[i]);
    }
    printf("\n共 %d 只宠物。\n", g_count);
}

static void search_pets(void)
{
    if (g_count == 0) {
        printf("\n（暂无记录）\n");
        return;
    }

    printf("\n--- 查找宠物 ---\n");
    printf(" 1. 按编号精确查找\n");
    printf(" 2. 按名字模糊查找\n");
    int mode = read_int("请选择（1-2）：", 1, 2);

    int found = 0;

    if (mode == 1) {
        int id = read_int("请输入编号：", 1000, 999999);
        int idx = find_by_id(id);
        if (idx >= 0) {
            print_header();
            print_pet(&g_pets[idx]);
            found = 1;
        }
    } else {
        char key[NAME_LEN];
        read_text("请输入名字关键字：", key, sizeof key, 1);

        print_header();
        for (int i = 0; i < g_count; i++) {
            if (strstr(g_pets[i].name, key) != NULL) {   /* 子串匹配 */
                print_pet(&g_pets[i]);
                found++;
            }
        }
    }

    if (found == 0) {
        printf("没有找到匹配的宠物。\n");
    } else if (mode == 2) {
        printf("\n共找到 %d 条。\n", found);
    }
}

static void modify_pet(void)
{
    if (g_count == 0) {
        printf("\n（暂无记录）\n");
        return;
    }

    printf("\n--- 修改宠物信息 ---\n");
    int id = read_int("请输入要修改的编号：", 1000, 999999);
    int idx = find_by_id(id);

    if (idx < 0) {
        printf("✗ 没有编号为 %d 的宠物。\n", id);
        return;
    }

    Pet *p = &g_pets[idx];
    printf("当前信息：\n");
    print_header();
    print_pet(p);

    printf("\n提示：直接回车表示「保持原值」。\n");

    char buf[64];

    /* 每项都先读进缓冲区，非空才覆盖 */
    read_text("新名字：", buf, sizeof buf, 0);
    if (buf[0] != '\0') snprintf(p->name, sizeof p->name, "%s", buf);

    read_text("新种类：", buf, sizeof buf, 0);
    if (buf[0] != '\0') snprintf(p->species, sizeof p->species, "%s", buf);

    read_text("新年龄：", buf, sizeof buf, 0);
    if (buf[0] != '\0') {
        int age = atoi(buf);
        if (age >= 0 && age <= 100) p->age = age;
        else printf("  ✗ 年龄不合法，保持原值。\n");
    }

    read_text("新体重：", buf, sizeof buf, 0);
    if (buf[0] != '\0') {
        double w = atof(buf);
        if (w > 0 && w <= 200) p->weight = w;
        else printf("  ✗ 体重不合法，保持原值。\n");
    }

    read_text("新主人：", buf, sizeof buf, 0);
    if (buf[0] != '\0') snprintf(p->owner, sizeof p->owner, "%s", buf);

    printf("✓ 修改完成。\n");
}

static void delete_pet(void)
{
    if (g_count == 0) {
        printf("\n（暂无记录）\n");
        return;
    }

    printf("\n--- 删除宠物 ---\n");
    int id = read_int("请输入要删除的编号：", 1000, 999999);
    int idx = find_by_id(id);

    if (idx < 0) {
        printf("✗ 没有编号为 %d 的宠物。\n", id);
        return;
    }

    printf("即将删除：%s（%s，主人 %s）\n",
           g_pets[idx].name, g_pets[idx].species, g_pets[idx].owner);

    if (!read_yes_no("确认删除？(y/n)：")) {
        printf("已取消。\n");
        return;
    }

    /* 后面的元素整体前移一位，完成删除 */
    for (int i = idx; i < g_count - 1; i++) {
        g_pets[i] = g_pets[i + 1];
    }
    g_count--;

    printf("✓ 已删除，当前共 %d 只宠物。\n", g_count);
}

/* ---------- 排序：qsort 需要「比较函数」 ---------- */

static int cmp_by_id(const void *a, const void *b)
{
    const Pet *x = (const Pet *)a;
    const Pet *y = (const Pet *)b;
    return (x->id > y->id) - (x->id < y->id);      /* 升序 */
}

static int cmp_by_age(const void *a, const void *b)
{
    const Pet *x = (const Pet *)a;
    const Pet *y = (const Pet *)b;
    if (x->age != y->age) return x->age - y->age;  /* 年龄小的在前 */
    return strcmp(x->name, y->name);               /* 年龄相同比名字 */
}

static int cmp_by_name(const void *a, const void *b)
{
    const Pet *x = (const Pet *)a;
    const Pet *y = (const Pet *)b;
    return strcmp(x->name, y->name);
}

static void sort_pets(void)
{
    if (g_count < 2) {
        printf("\n（记录太少，无需排序）\n");
        return;
    }

    printf("\n--- 排序 ---\n");
    printf(" 1. 按编号\n");
    printf(" 2. 按年龄（小 → 大）\n");
    printf(" 3. 按名字\n");
    int mode = read_int("请选择（1-3）：", 1, 3);

    if (mode == 1) {
        qsort(g_pets, (size_t)g_count, sizeof(Pet), cmp_by_id);
    } else if (mode == 2) {
        qsort(g_pets, (size_t)g_count, sizeof(Pet), cmp_by_age);
    } else {
        qsort(g_pets, (size_t)g_count, sizeof(Pet), cmp_by_name);
    }

    printf("✓ 排序完成。\n");
    list_pets();
}

static void show_stats(void)
{
    if (g_count == 0) {
        printf("\n（暂无记录）\n");
        return;
    }

    static const char *kinds[] = { "狗", "猫", "鸟", "兔", "鱼", "仓鼠" };
    const int kind_count = (int)(sizeof kinds / sizeof kinds[0]);

    int    counts[6] = { 0 };
    int    sum_age   = 0;
    double sum_weight = 0.0;
    int    heaviest = 0;              /* 记录最重宠物的下标 */

    for (int i = 0; i < g_count; i++) {
        for (int k = 0; k < kind_count; k++) {
            if (strcmp(g_pets[i].species, kinds[k]) == 0) {
                counts[k]++;
                break;
            }
        }
        sum_age    += g_pets[i].age;
        sum_weight += g_pets[i].weight;
        if (g_pets[i].weight > g_pets[heaviest].weight) heaviest = i;
    }

    printf("\n--- 统计信息 ---\n");
    printf("总数量    ：%d 只\n", g_count);
    printf("平均年龄  ：%.1f 岁\n", (double)sum_age / g_count);
    printf("平均体重  ：%.1f kg\n", sum_weight / g_count);
    printf("最重的宠物：%s（%.1f kg）\n",
           g_pets[heaviest].name, g_pets[heaviest].weight);

    printf("\n按种类统计：\n");
    for (int k = 0; k < kind_count; k++) {
        if (counts[k] > 0) {
            print_pad(kinds[k], 8);
            printf("%d 只\n", counts[k]);
        }
    }
}

/* =========================================================
   5. 菜单与主函数
   ========================================================= */

static void print_menu(void)
{
    putchar('\n');
    printf("============ 宠物管理系统 ============\n");
    printf("  当前记录：%d / %d\n", g_count, MAX_PETS);
    printf("--------------------------------------\n");
    printf("  1. 添加宠物      2. 查看全部\n");
    printf("  3. 查找宠物      4. 修改信息\n");
    printf("  5. 删除宠物      6. 排序显示\n");
    printf("  7. 统计信息      8. 保存数据\n");
    printf("  0. 退出（自动保存）\n");
    printf("======================================\n");
}

int main(void)
{
    load_pets();

    for (;;) {
        print_menu();
        int choice = read_int("请选择（0-8）：", 0, 8);

        switch (choice) {
        case 1: add_pet();     break;
        case 2: list_pets();   break;
        case 3: search_pets(); break;
        case 4: modify_pet();  break;
        case 5: delete_pet();  break;
        case 6: sort_pets();   break;
        case 7: show_stats();  break;
        case 8: save_pets();   break;
        case 0:
            save_pets();
            printf("再见！数据已保存到 %s\n", DATA_FILE);
            return 0;
        default:
            break;            /* read_int 已限制范围，这里不会走到 */
        }
    }
}
