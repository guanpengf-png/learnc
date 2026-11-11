/*
 * =============================================================================
 * 学生成绩管理系统（教学版）
 * =============================================================================
 * 练习重点：
 *   - 结构体 struct
 *   - 单链表：增、删、改、查、遍历
 *   - 二级指针 Node **：删除头节点时能改「头指针本身」
 *   - 文件读写：文本保存 / 加载，程序关掉数据不丢
 *
 * 编译运行：
 *   cd day7/student_manager && make && ./student
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 32
#define DATA_FILE "students.txt" /* 默认保存文件名 */

/* -----------------------------------------------------------------------------
 * 学生信息：一个链表节点里装一个学生
 * ----------------------------------------------------------------------------- */
typedef struct Student {
    int id;                 /* 学号，唯一 */
    char name[NAME_LEN];    /* 姓名 */
    float chinese;          /* 语文 */
    float math;             /* 数学 */
    float english;          /* 英语 */
    struct Student *next;   /* 指向下一个学生节点；最后一个为 NULL */
} Student;

/* =============================================================================
 * 工具函数
 * ============================================================================= */

/*
 * calc_avg —— 算三科平均分
 * 参数：指向某个学生节点的指针（只读）
 * 返回：平均分
 */
static float calc_avg(const Student *s)
{
    return (s->chinese + s->math + s->english) / 3.0f;
}

/*
 * clear_stdin —— 清空输入缓冲区里残留的换行等字符
 * 干什么：scanf 读完数字后常留下 '\n'，再用 fgets 会读到空行。
 */
static void clear_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/*
 * read_line —— 读一整行到 buf，去掉末尾 '\n'
 * 返回：成功 1，失败（EOF）0
 */
static int read_line(char *buf, size_t n)
{
    if (!fgets(buf, (int)n, stdin)) {
        return 0;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
    return 1;
}

/* =============================================================================
 * 链表：查找
 * ============================================================================= */

/*
 * find_by_id —— 按学号查找
 *
 * 干什么：从头开始沿 next 走，找到 id 相等的节点。
 *
 * 参数：
 *   head —— 链表头指针（第一个节点的地址；空表则为 NULL）
 *   id   —— 要找的学号
 * 返回：
 *   找到：指向该 Student 节点的指针
 *   没找到：NULL
 *
 * 指针点：
 *   p = p->next 就是「走到下一个节点」。
 */
static Student *find_by_id(Student *head, int id)
{
    Student *p = head;
    while (p != NULL) {
        if (p->id == id) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

/* =============================================================================
 * 链表：增加（头插）
 * ============================================================================= */

/*
 * student_create —— 在堆上 new 一个学生节点并填数据
 *
 * 干什么：malloc 一块 Student，写入字段，next 先置 NULL。
 * 返回：新节点指针；内存不足则返回 NULL。
 */
static Student *student_create(int id, const char *name, float chinese, float math,
                               float english)
{
    Student *node = (Student *)malloc(sizeof(Student));
    if (!node) {
        return NULL;
    }
    node->id = id;
    /* 安全拷贝姓名，防止超过 NAME_LEN */
    strncpy(node->name, name, NAME_LEN - 1);
    node->name[NAME_LEN - 1] = '\0';
    node->chinese = chinese;
    node->math = math;
    node->english = english;
    node->next = NULL;
    return node;
}

/*
 * list_add —— 在链表头部插入一名学生（头插法）
 *
 * 干什么：
 *   1. 学号已存在则拒绝
 *   2. create 新节点
 *   3. 新节点的 next 指向原来的头
 *   4. 头指针改成新节点（所以参数是 Student **head）
 *
 * 参数：
 *   head —— 「头指针变量」的地址。因为要改 head 本身，必须二级指针。
 *           调用写作：list_add(&head, ...)
 *
 * 返回：1 成功，0 失败（重复学号或 malloc 失败）
 *
 * 图示（头插）：
 *   原来：head -> A -> B -> NULL
 *   插入 X 后：head -> X -> A -> B -> NULL
 */
static int list_add(Student **head, int id, const char *name, float chinese, float math,
                    float english)
{
    if (find_by_id(*head, id) != NULL) {
        printf("错误：学号 %d 已存在。\n", id);
        return 0;
    }

    Student *node = student_create(id, name, chinese, math, english);
    if (!node) {
        printf("错误：内存分配失败。\n");
        return 0;
    }

    node->next = *head; /* 新节点接上旧链表 */
    *head = node;       /* 头指针指向新节点 —— 改的是调用者的 head */
    return 1;
}

/* =============================================================================
 * 链表：删除
 * ============================================================================= */

/*
 * list_delete —— 按学号删除一名学生
 *
 * 干什么：
 *   找到目标节点，让「前驱的 next」跳过它，再 free 目标。
 *   若删的是头节点，必须改 *head，所以仍用 Student **。
 *
 * 参数：head 头指针的地址；id 学号
 * 返回：1 删成功，0 没找到
 *
 * 二级指针删除头节点（经典考点）：
 *   *head = target->next;
 *   free(target);
 */
static int list_delete(Student **head, int id)
{
    Student *cur = *head;
    Student *prev = NULL;

    while (cur != NULL) {
        if (cur->id == id) {
            if (prev == NULL) {
                /* 删的是第一个节点：更新头指针 */
                *head = cur->next;
            } else {
                /* 删的是中间或尾：前驱跳过 cur */
                prev->next = cur->next;
            }
            free(cur); /* 释放堆内存，否则泄漏 */
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/* =============================================================================
 * 链表：修改
 * ============================================================================= */

/*
 * list_update —— 按学号修改姓名和三科成绩
 *
 * 干什么：先 find，找到就改字段。链表结构（next）不用动。
 * 返回：1 成功，0 没找到
 */
static int list_update(Student *head, int id, const char *name, float chinese, float math,
                       float english)
{
    Student *p = find_by_id(head, id);
    if (!p) {
        return 0;
    }
    strncpy(p->name, name, NAME_LEN - 1);
    p->name[NAME_LEN - 1] = '\0';
    p->chinese = chinese;
    p->math = math;
    p->english = english;
    return 1;
}

/* =============================================================================
 * 链表：查 / 遍历打印
 * ============================================================================= */

/*
 * list_print_one —— 打印单个学生（含平均分）
 */
static void list_print_one(const Student *s)
{
    printf("学号:%-6d 姓名:%-10s 语文:%6.1f 数学:%6.1f 英语:%6.1f 平均:%6.1f\n", s->id,
           s->name, s->chinese, s->math, s->english, calc_avg(s));
}

/*
 * list_print_all —— 遍历整条链表打印
 *
 * 干什么：p 从 head 走到 NULL，每走一步打印一个节点。
 * 空表则提示。
 */
static void list_print_all(const Student *head)
{
    if (!head) {
        printf("（当前没有学生记录）\n");
        return;
    }
    printf("---------- 全部学生 ----------\n");
    const Student *p = head;
    int n = 0;
    while (p != NULL) {
        list_print_one(p);
        p = p->next;
        n++;
    }
    printf("共 %d 人\n", n);
}

/*
 * list_search —— 按学号查询并打印
 */
static void list_search(Student *head, int id)
{
    Student *p = find_by_id(head, id);
    if (!p) {
        printf("未找到学号 %d\n", id);
        return;
    }
    printf("查询结果：\n");
    list_print_one(p);
}

/* =============================================================================
 * 链表：整表释放
 * ============================================================================= */

/*
 * list_free —— 释放所有节点，避免内存泄漏
 *
 * 干什么：先记下 next，再 free 当前，否则 next 丢了就再也走不到后面。
 * 最后 *head = NULL。
 */
static void list_free(Student **head)
{
    Student *p = *head;
    while (p != NULL) {
        Student *next = p->next;
        free(p);
        p = next;
    }
    *head = NULL;
}

/* =============================================================================
 * 文件读写
 * ============================================================================= */

/*
 * list_save —— 把链表写到文本文件
 *
 * 干什么：
 *   每行一个学生：id name chinese math english
 *   用 fprintf 写；空格分隔，姓名约定不含空格（教学简化）。
 *
 * 返回：1 成功，0 失败（打不开文件）
 */
static int list_save(const Student *head, const char *path)
{
    FILE *fp = fopen(path, "w"); /* 写模式：覆盖旧文件 */
    if (!fp) {
        perror("保存失败");
        return 0;
    }

    const Student *p = head;
    while (p != NULL) {
        fprintf(fp, "%d %s %.1f %.1f %.1f\n", p->id, p->name, p->chinese, p->math,
                p->english);
        p = p->next;
    }

    fclose(fp);
    return 1;
}

/*
 * list_load —— 从文本文件读入，重建链表
 *
 * 干什么：
 *   先清空旧链表，再逐行 fscanf，每读到一个学生就 list_add。
 *   文件不存在时当作空表，不算错误（第一次运行常见）。
 *
 * 返回：成功加载的人数；文件不存在返回 0
 */
static int list_load(Student **head, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) {
        /* 第一次没有文件是正常的 */
        return 0;
    }

    list_free(head); /* 加载前丢掉内存里旧数据，避免和文件两套 */

    int id;
    char name[NAME_LEN];
    float chinese, math, english;
    int count = 0;

    /*
     * fscanf 按格式读：
     *   %d 学号，%31s 姓名（最多 31 字符，留给 '\0'），三个 float
     * 返回成功读入的项数，期望为 5
     */
    while (fscanf(fp, "%d %31s %f %f %f", &id, name, &chinese, &math, &english) == 5) {
        if (list_add(head, id, name, chinese, math, english)) {
            count++;
        }
    }

    fclose(fp);
    return count;
}

/* =============================================================================
 * 菜单交互（把「读用户输入」和「调链表 API」接起来）
 * ============================================================================= */

static void menu_add(Student **head)
{
    int id;
    char name[NAME_LEN];
    float chinese, math, english;

    printf("学号: ");
    if (scanf("%d", &id) != 1) {
        printf("输入无效\n");
        clear_stdin();
        return;
    }
    clear_stdin();

    printf("姓名(不要有空格): ");
    if (!read_line(name, sizeof name) || name[0] == '\0') {
        printf("姓名不能为空\n");
        return;
    }

    printf("语文 数学 英语（空格分隔）: ");
    if (scanf("%f %f %f", &chinese, &math, &english) != 3) {
        printf("成绩输入无效\n");
        clear_stdin();
        return;
    }
    clear_stdin();

    if (list_add(head, id, name, chinese, math, english)) {
        printf("添加成功。\n");
    }
}

static void menu_delete(Student **head)
{
    int id;
    printf("要删除的学号: ");
    if (scanf("%d", &id) != 1) {
        printf("输入无效\n");
        clear_stdin();
        return;
    }
    clear_stdin();

    if (list_delete(head, id)) {
        printf("已删除学号 %d\n", id);
    } else {
        printf("未找到学号 %d\n", id);
    }
}

static void menu_update(Student *head)
{
    int id;
    char name[NAME_LEN];
    float chinese, math, english;

    printf("要修改的学号: ");
    if (scanf("%d", &id) != 1) {
        printf("输入无效\n");
        clear_stdin();
        return;
    }
    clear_stdin();

    if (!find_by_id(head, id)) {
        printf("未找到学号 %d\n", id);
        return;
    }

    printf("新姓名: ");
    if (!read_line(name, sizeof name) || name[0] == '\0') {
        printf("姓名不能为空\n");
        return;
    }
    printf("新成绩 语文 数学 英语: ");
    if (scanf("%f %f %f", &chinese, &math, &english) != 3) {
        printf("成绩输入无效\n");
        clear_stdin();
        return;
    }
    clear_stdin();

    if (list_update(head, id, name, chinese, math, english)) {
        printf("修改成功。\n");
    }
}

static void menu_search(Student *head)
{
    int id;
    printf("要查询的学号: ");
    if (scanf("%d", &id) != 1) {
        printf("输入无效\n");
        clear_stdin();
        return;
    }
    clear_stdin();
    list_search(head, id);
}

static void print_menu(void)
{
    puts("\n===== 学生成绩管理系统 =====");
    puts("1. 添加学生");
    puts("2. 删除学生");
    puts("3. 修改成绩");
    puts("4. 按学号查询");
    puts("5. 显示全部");
    puts("6. 保存到文件");
    puts("7. 从文件加载");
    puts("0. 退出");
    printf("请选择: ");
}

/* =============================================================================
 * main
 * ============================================================================= */

/*
 * main —— 程序入口
 *
 * 流程：
 *   1. head = NULL 表示空链表
 *   2. 启动时尝试从 DATA_FILE 自动加载
 *   3. 循环菜单，直到选 0
 *   4. 退出前可自动保存；list_free 释放内存
 */
int main(void)
{
    Student *head = NULL; /* 头指针：指向第一个节点；空表为 NULL */
    int choice;

    int n = list_load(&head, DATA_FILE);
    if (n > 0) {
        printf("已从 %s 加载 %d 名学生。\n", DATA_FILE, n);
    } else {
        printf("未找到 %s，从空表开始。\n", DATA_FILE);
    }

    for (;;) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("请输入数字菜单项。\n");
            clear_stdin();
            continue;
        }
        clear_stdin();

        switch (choice) {
        case 1:
            menu_add(&head); /* 传入 &head：添加时可能改头指针 */
            break;
        case 2:
            menu_delete(&head);
            break;
        case 3:
            menu_update(head); /* 修改不改头指针，传一级即可 */
            break;
        case 4:
            menu_search(head);
            break;
        case 5:
            list_print_all(head);
            break;
        case 6:
            if (list_save(head, DATA_FILE)) {
                printf("已保存到 %s\n", DATA_FILE);
            }
            break;
        case 7:
            n = list_load(&head, DATA_FILE);
            printf("已从文件加载，插入 %d 人。\n", n);
            list_print_all(head);
            break;
        case 0:
            if (list_save(head, DATA_FILE)) {
                printf("退出前已自动保存到 %s\n", DATA_FILE);
            }
            list_free(&head);
            puts("再见。");
            return 0;
        default:
            puts("无效选项。");
            break;
        }
    }
}
