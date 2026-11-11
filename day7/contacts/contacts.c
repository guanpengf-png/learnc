/*
 * =============================================================================
 * 通讯录命令行（教学版）
 * =============================================================================
 * 练习重点：
 *   - 结构体 + 单链表
 *   - 插入（头插 / 按姓名有序插入）
 *   - 按姓名 / 电话删除
 *   - 按名字模糊查找
 *   - 按姓名排序（链表归并排序，不另开数组）
 *   - 文件读写
 *
 * 编译运行：
 *   cd day7/contacts && make && ./contacts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* strcasecmp */
#include <ctype.h>

#define NAME_LEN 32
#define PHONE_LEN 20
#define EMAIL_LEN 48
#define DATA_FILE "contacts.txt"

/* -----------------------------------------------------------------------------
 * 联系人节点
 * ----------------------------------------------------------------------------- */
typedef struct Contact {
    char name[NAME_LEN];     /* 姓名 */
    char phone[PHONE_LEN];   /* 电话（当作唯一键之一，删除可用） */
    char email[EMAIL_LEN];   /* 邮箱，可为空字符串 */
    struct Contact *next;    /* 下一个节点 */
} Contact;

/* =============================================================================
 * 输入工具
 * ============================================================================= */

static void clear_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

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

/* 不区分大小写的字符串比较：<0 / 0 / >0 */
static int name_cmp(const char *a, const char *b)
{
    return strcasecmp(a, b);
}

/* =============================================================================
 * 创建节点 / 查找
 * ============================================================================= */

/*
 * contact_create —— 堆上分配一个联系人节点并填字段
 */
static Contact *contact_create(const char *name, const char *phone, const char *email)
{
    Contact *node = (Contact *)malloc(sizeof(Contact));
    if (!node) {
        return NULL;
    }
    strncpy(node->name, name, NAME_LEN - 1);
    node->name[NAME_LEN - 1] = '\0';
    strncpy(node->phone, phone, PHONE_LEN - 1);
    node->phone[PHONE_LEN - 1] = '\0';
    strncpy(node->email, email, EMAIL_LEN - 1);
    node->email[EMAIL_LEN - 1] = '\0';
    node->next = NULL;
    return node;
}

/*
 * find_by_phone —— 电话唯一，精确查找
 */
static Contact *find_by_phone(Contact *head, const char *phone)
{
    for (Contact *p = head; p; p = p->next) {
        if (strcmp(p->phone, phone) == 0) {
            return p;
        }
    }
    return NULL;
}

/*
 * find_by_name_exact —— 姓名精确匹配（排序插入时查重可用）
 */
static Contact *find_by_name_exact(Contact *head, const char *name)
{
    for (Contact *p = head; p; p = p->next) {
        if (name_cmp(p->name, name) == 0) {
            return p;
        }
    }
    return NULL;
}

/* =============================================================================
 * 插入
 * ============================================================================= */

/*
 * list_insert_head —— 头插：新节点成为新的头
 * 参数 head 是二级指针，因为要改调用者的头指针。
 */
static int list_insert_head(Contact **head, const char *name, const char *phone,
                            const char *email)
{
    if (find_by_phone(*head, phone)) {
        printf("错误：电话 %s 已存在。\n", phone);
        return 0;
    }
    Contact *node = contact_create(name, phone, email);
    if (!node) {
        printf("错误：内存不足。\n");
        return 0;
    }
    node->next = *head;
    *head = node;
    return 1;
}

/*
 * list_insert_sorted —— 按姓名有序插入（升序，不区分大小写）
 *
 * 干什么：
 *   找到第一个「姓名 > 新姓名」的位置，插到它前面。
 *   若应插到最前，等价于改 *head。
 *
 * 图示：
 *   ... -> prev -> cur -> ...
 *   插入 node 后：
 *   ... -> prev -> node -> cur -> ...
 */
static int list_insert_sorted(Contact **head, const char *name, const char *phone,
                              const char *email)
{
    if (find_by_phone(*head, phone)) {
        printf("错误：电话 %s 已存在。\n", phone);
        return 0;
    }
    if (find_by_name_exact(*head, name)) {
        printf("提示：已有同名联系人，仍按电话区分继续添加。\n");
    }

    Contact *node = contact_create(name, phone, email);
    if (!node) {
        printf("错误：内存不足。\n");
        return 0;
    }

    /* 空表，或新名字应排在第一个之前 */
    if (*head == NULL || name_cmp(name, (*head)->name) <= 0) {
        node->next = *head;
        *head = node;
        return 1;
    }

    Contact *prev = *head;
    Contact *cur = (*head)->next;
    while (cur != NULL && name_cmp(name, cur->name) > 0) {
        prev = cur;
        cur = cur->next;
    }
    /* 插到 prev 与 cur 之间 */
    prev->next = node;
    node->next = cur;
    return 1;
}

/* =============================================================================
 * 删除
 * ============================================================================= */

/*
 * list_delete_by_phone —— 按电话删除（精确）
 * 用二级指针改头：删头时 *head = cur->next
 */
static int list_delete_by_phone(Contact **head, const char *phone)
{
    Contact *cur = *head;
    Contact *prev = NULL;

    while (cur != NULL) {
        if (strcmp(cur->phone, phone) == 0) {
            if (prev == NULL) {
                *head = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/*
 * list_delete_by_name —— 按姓名删除「第一个」匹配的联系人
 * 同名多人时只删第一个；需要全删可循环调用。
 */
static int list_delete_by_name(Contact **head, const char *name)
{
    Contact *cur = *head;
    Contact *prev = NULL;

    while (cur != NULL) {
        if (name_cmp(cur->name, name) == 0) {
            if (prev == NULL) {
                *head = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/* =============================================================================
 * 按名字查找（支持子串模糊匹配）
 * ============================================================================= */

/*
 * list_search_by_name —— 打印所有姓名包含 keyword 的联系人（不区分大小写）
 *
 * 干什么：
 *   把 name 和 keyword 都转成小写临时缓冲，再 strstr 判断「是否包含」。
 *   这是通讯录里最常用的「按名字搜」。
 *
 * 返回：匹配条数
 */
static int list_search_by_name(const Contact *head, const char *keyword)
{
    if (!keyword || keyword[0] == '\0') {
        printf("关键词不能为空。\n");
        return 0;
    }

    char key_lower[NAME_LEN];
    strncpy(key_lower, keyword, NAME_LEN - 1);
    key_lower[NAME_LEN - 1] = '\0';
    for (char *c = key_lower; *c; c++) {
        *c = (char)tolower((unsigned char)*c);
    }

    int hits = 0;
    printf("---------- 搜索「%s」----------\n", keyword);
    for (const Contact *p = head; p; p = p->next) {
        char name_lower[NAME_LEN];
        strncpy(name_lower, p->name, NAME_LEN - 1);
        name_lower[NAME_LEN - 1] = '\0';
        for (char *c = name_lower; *c; c++) {
            *c = (char)tolower((unsigned char)*c);
        }

        if (strstr(name_lower, key_lower) != NULL) {
            printf("姓名:%-12s 电话:%-14s 邮箱:%s\n", p->name, p->phone, p->email);
            hits++;
        }
    }
    if (hits == 0) {
        printf("（无匹配）\n");
    } else {
        printf("共 %d 条\n", hits);
    }
    return hits;
}

/* =============================================================================
 * 排序：链表归并排序（稳定、O(n log n)、只用指针）
 * ============================================================================= */

/*
 * split_half —— 快慢指针把链表拆成前后两半
 *
 * 干什么：
 *   slow 一次走 1 步，fast 一次走 2 步；
 *   fast 到尾时，slow 大约在中间。
 *   在 slow 处断开：*front = 原头，*back = 后半头。
 */
static void split_half(Contact *source, Contact **front, Contact **back)
{
    if (source == NULL || source->next == NULL) {
        *front = source;
        *back = NULL;
        return;
    }

    Contact *slow = source;
    Contact *fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL; /* 前半截尾 */
}

/*
 * merge_sorted —— 合并两条「已按姓名排好序」的链表
 * 返回新的头指针。
 */
static Contact *merge_sorted(Contact *a, Contact *b)
{
    if (!a) {
        return b;
    }
    if (!b) {
        return a;
    }

    Contact *result = NULL;
    if (name_cmp(a->name, b->name) <= 0) {
        result = a;
        result->next = merge_sorted(a->next, b);
    } else {
        result = b;
        result->next = merge_sorted(a, b->next);
    }
    return result;
}

/*
 * merge_sort —— 对单链表按姓名归并排序
 *
 * 干什么：
 *   分：拆两半；治：递归排序；合：merge_sorted。
 *   不额外开数组，只改 next 指针 —— 练链表指针的好例子。
 *
 * 参数：指向头指针的地址；排序后 *head 指向新头。
 */
static void merge_sort(Contact **head)
{
    Contact *h = *head;
    if (h == NULL || h->next == NULL) {
        return; /* 0 或 1 个节点，已有序 */
    }

    Contact *a = NULL;
    Contact *b = NULL;
    split_half(h, &a, &b);
    merge_sort(&a);
    merge_sort(&b);
    *head = merge_sorted(a, b);
}

/* =============================================================================
 * 遍历 / 释放
 * ============================================================================= */

static void list_print_all(const Contact *head)
{
    if (!head) {
        printf("（通讯录为空）\n");
        return;
    }
    printf("---------- 全部联系人 ----------\n");
    int n = 0;
    for (const Contact *p = head; p; p = p->next) {
        printf("%2d. 姓名:%-12s 电话:%-14s 邮箱:%s\n", ++n, p->name, p->phone, p->email);
    }
    printf("共 %d 人\n", n);
}

static void list_free(Contact **head)
{
    Contact *p = *head;
    while (p) {
        Contact *next = p->next;
        free(p);
        p = next;
    }
    *head = NULL;
}

/* =============================================================================
 * 文件：格式每行 name phone email（姓名/电话不含空格；邮箱可含 @）
 * ============================================================================= */

static int list_save(const Contact *head, const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror("保存失败");
        return 0;
    }
    for (const Contact *p = head; p; p = p->next) {
        /* 邮箱为空时写 "-" 占位，加载时再还原 */
        const char *em = (p->email[0] != '\0') ? p->email : "-";
        fprintf(fp, "%s %s %s\n", p->name, p->phone, em);
    }
    fclose(fp);
    return 1;
}

static int list_load(Contact **head, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) {
        return 0;
    }

    list_free(head);

    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];
    int count = 0;
    while (fscanf(fp, "%31s %19s %47s", name, phone, email) == 3) {
        if (strcmp(email, "-") == 0) {
            email[0] = '\0';
        }
        /* 加载时用有序插入，打开文件后列表已按姓名排好 */
        if (list_insert_sorted(head, name, phone, email)) {
            count++;
        }
    }
    fclose(fp);
    return count;
}

/* =============================================================================
 * 菜单
 * ============================================================================= */

static void read_contact_fields(char *name, char *phone, char *email)
{
    printf("姓名(无空格): ");
    read_line(name, NAME_LEN);
    printf("电话(无空格): ");
    read_line(phone, PHONE_LEN);
    printf("邮箱(无空格，可留空直接回车): ");
    read_line(email, EMAIL_LEN);
}

static void menu_add(Contact **head, int sorted)
{
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];
    read_contact_fields(name, phone, email);
    if (name[0] == '\0' || phone[0] == '\0') {
        printf("姓名和电话不能为空。\n");
        return;
    }
    int ok = sorted ? list_insert_sorted(head, name, phone, email)
                    : list_insert_head(head, name, phone, email);
    if (ok) {
        printf("已添加。\n");
    }
}

static void menu_delete(Contact **head)
{
    puts("1) 按电话删除  2) 按姓名删除");
    printf("选择: ");
    int mode;
    if (scanf("%d", &mode) != 1) {
        clear_stdin();
        printf("输入无效\n");
        return;
    }
    clear_stdin();

    char buf[NAME_LEN];
    if (mode == 1) {
        printf("电话: ");
        read_line(buf, sizeof buf);
        if (list_delete_by_phone(head, buf)) {
            printf("已删除。\n");
        } else {
            printf("未找到该电话。\n");
        }
    } else if (mode == 2) {
        printf("姓名: ");
        read_line(buf, sizeof buf);
        if (list_delete_by_name(head, buf)) {
            printf("已删除同名第一条。\n");
        } else {
            printf("未找到该姓名。\n");
        }
    } else {
        printf("无效选项。\n");
    }
}

static void print_menu(void)
{
    puts("\n===== 通讯录 =====");
    puts("1. 头插添加");
    puts("2. 按姓名有序插入");
    puts("3. 删除");
    puts("4. 按名字搜索（支持部分匹配）");
    puts("5. 显示全部");
    puts("6. 按姓名排序（归并）");
    puts("7. 保存到文件");
    puts("8. 从文件加载");
    puts("0. 退出");
    printf("请选择: ");
}

int main(void)
{
    Contact *head = NULL;
    int n = list_load(&head, DATA_FILE);
    if (n > 0) {
        printf("已从 %s 加载 %d 人。\n", DATA_FILE, n);
    } else {
        printf("未找到 %s，从空通讯录开始。\n", DATA_FILE);
    }

    for (;;) {
        print_menu();
        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            puts("请输入数字。");
            continue;
        }
        clear_stdin();

        switch (choice) {
        case 1:
            menu_add(&head, 0);
            break;
        case 2:
            menu_add(&head, 1);
            break;
        case 3:
            menu_delete(&head);
            break;
        case 4: {
            char key[NAME_LEN];
            printf("名字关键词: ");
            read_line(key, sizeof key);
            list_search_by_name(head, key);
            break;
        }
        case 5:
            list_print_all(head);
            break;
        case 6:
            merge_sort(&head);
            printf("已按姓名排序。\n");
            list_print_all(head);
            break;
        case 7:
            if (list_save(head, DATA_FILE)) {
                printf("已保存到 %s\n", DATA_FILE);
            }
            break;
        case 8:
            n = list_load(&head, DATA_FILE);
            printf("加载 %d 人。\n", n);
            list_print_all(head);
            break;
        case 0:
            list_save(head, DATA_FILE);
            list_free(&head);
            puts("再见。");
            return 0;
        default:
            puts("无效选项。");
            break;
        }
    }
}
