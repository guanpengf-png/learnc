/*
 * 章节：单链表
 * 知识点：
 *   链表是线性结构。每个结点有数据域 data 和下一个结点的地址 next。
 *   结点用 malloc 单独申请，所以物理上不必连续。链表头 header 保存第一个结点的地址，空链表的头是 NULL。
 *   在已知前一个结点时，插入和删除只改指针，不用搬动别的元素。
 *   但是按下标找结点必须从头走到那里，不能像数组一样直接算出地址。随机访问是 O(n)。
 *   每个结点都多存一个指针，同样的内存能放下的数据比紧凑的数组少。
 *   讲义的 get_value 和 edit_value 用 index > length 判断越界。
 *   下标等于 length 时其实已经没有这个结点，会顺着空指针走。正确条件是 index >= length。
 *   释放时要先记住 next，再 free 当前结点。先 free 再读 next 是已经释放的内存。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *header;
    size_t length;
} LinkedList;

static void init_linked_list(LinkedList *list)
{
    list->header = NULL;
    list->length = 0;
}

static void destroy_list(LinkedList *list)
{
    Node *current = list->header;
    while (current != NULL) {
        Node *doomed = current;
        current = current->next;
        free(doomed);
    }
    list->header = NULL;
    list->length = 0;
}

static size_t get_length(const LinkedList *list)
{
    return list->length;
}

static Node *get_prev_node(const LinkedList *list, size_t index)
{
    Node *prev = list->header;
    for (size_t i = 1; i < index; i++) {
        prev = prev->next;
    }
    return prev;
}

static bool insert_at(LinkedList *list, size_t index, int element)
{
    if (index > list->length) {
        return false;
    }
    Node *node = malloc(sizeof *node);
    if (node == NULL) {
        return false;
    }
    node->data = element;
    if (index == 0) {
        node->next = list->header;
        list->header = node;
    } else {
        Node *prev = get_prev_node(list, index);
        node->next = prev->next;
        prev->next = node;
    }
    list->length++;
    return true;
}

static bool insert_end(LinkedList *list, int element)
{
    return insert_at(list, list->length, element);
}

static bool delete_at(LinkedList *list, size_t index, int *deleted)
{
    if (index >= list->length) {
        return false;
    }
    Node *doomed = NULL;
    if (index == 0) {
        doomed = list->header;
        list->header = list->header->next;
    } else {
        Node *prev = get_prev_node(list, index);
        doomed = prev->next;
        prev->next = doomed->next;
    }
    *deleted = doomed->data;
    free(doomed);
    list->length--;
    return true;
}

static bool delete_end(LinkedList *list, int *deleted)
{
    if (list->length == 0) {
        return false;
    }
    return delete_at(list, list->length - 1, deleted);
}

static bool get_value(const LinkedList *list, size_t index, int *element)
{
    if (index >= list->length) {
        return false;
    }
    if (index == 0) {
        *element = list->header->data;
        return true;
    }
    *element = get_prev_node(list, index)->next->data;
    return true;
}

static bool edit_value(LinkedList *list, size_t index, int element)
{
    if (index >= list->length) {
        return false;
    }
    if (index == 0) {
        list->header->data = element;
        return true;
    }
    get_prev_node(list, index)->next->data = element;
    return true;
}

static void print_list(const LinkedList *list)
{
    if (list->length == 0) {
        printf("(空)\n");
        return;
    }
    for (const Node *node = list->header; node != NULL; node = node->next) {
        printf("%d ", node->data);
    }
    printf("\n");
}

int main(void)
{
    LinkedList list;
    init_linked_list(&list);
    insert_at(&list, 0, 20);
    insert_at(&list, 0, 10);
    printf("头部插入两次后，长度 %zu\n", get_length(&list));
    insert_end(&list, 30);
    printf("尾部追加 30 后，长度 %zu\n", get_length(&list));
    print_list(&list);

    int deleted = 0;
    delete_at(&list, 2, &deleted);
    printf("删除下标 2，删掉的是 %d\n", deleted);
    delete_end(&list, &deleted);
    printf("删除尾部，删掉的是 %d\n", deleted);
    printf("长度 %zu\n", get_length(&list));

    int element = 0;
    get_value(&list, 0, &element);
    printf("下标 0 的值: %d\n", element);
    edit_value(&list, 0, 100);
    print_list(&list);

    printf("越界读取: %s\n", get_value(&list, 5, &element) ? "成功" : "失败");
    destroy_list(&list);
    printf("释放后再删尾部: %s\n", delete_end(&list, &deleted) ? "成功" : "失败");
    return 0;
}
