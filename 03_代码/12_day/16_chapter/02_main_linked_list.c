#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// 定义结点的结构体类型
typedef struct Node
{
  int data;          // 存储数据的
  struct Node *next; // 结点类型的指针,指向的下一个结点的内存地址
} Node;
// 定义链表的结构体类型
typedef struct
{
  size_t length; // 结点的个数,长度
  Node *header;  // 表头,应该指向第一个结点的内存地址
} LinkedList;
// 初始化链表:对成员进行初始化赋值的函数
void init_linked_list(LinkedList *list)
{
  list->length = 0;    // 默认初始化长度为0
  list->header = NULL; // 默认为空指针
}
// 释放链表的函数
void destroy_linked_list(LinkedList *list)
{
  // 获取第一个位置的结点
  Node *current_node = list->header;
  while (current_node != NULL)
  {
    Node *free_node = current_node;    // 先保存当前的结点
    current_node = current_node->next; // 查找下一个位置的结点
    free(free_node);                   // 释放当前位置的结点
  }
  list->length = 0;    // 重置长度
  list->header = NULL; // 重置为空指针
}
// 获取长度的函数
size_t get_length(LinkedList *list)
{
  return list->length;
}
// 根据当前位置查找前一个位置的结点
Node *find_prev_node(LinkedList *list, size_t index)
{
  Node *prev_node = list->header; // 先把第一个结点找到
  for (size_t i = 1; i < index; i++)
    prev_node = prev_node->next;
  return prev_node;
}
// 向指定位置插入数据的函数
void insert_at(LinkedList *list, size_t index, int element)
{
  // 先判断位置是否超出范围
  if (index > list->length)
    return;
  // 分配内存空间,设置新结点指向及存储数据操作
  Node *new_node = (Node *)malloc(sizeof(Node));
  // 判断位置
  if (index == 0)
  {
    new_node->next = list->header; // 把第一个结点的内存地址存储到新结点的next指针中
    list->header = new_node;       // 设置链表头指向新结点的内存地址
  }
  else
  {
    // 先找当前位置的前一个结点
    Node *prev_node = find_prev_node(list, index);
    new_node->next = prev_node->next; // 把当前位置的节点的内存地址存储到新结点的next指针中
    prev_node->next = new_node;       // 设置前一个位置的结点的next指针指向新结点的内存地址
  }
  new_node->data = element; // 把新数据存储到新的结点中
  list->length++;           // 更新长度加1
}
// 向尾部位置追加数据的函数
void insert_end(LinkedList *list, int element)
{
  insert_at(list, list->length, element);
}
// 从指定位置删除数据的函数
bool delete_at(LinkedList *list, size_t index, int *del_element)
{
  // 判断位置是否超出范围
  if (index >= list->length)
    return false;
  Node *del_node = NULL; // 用来存储要删除的结点
  if (index == 0)
  {
    del_node = list->header;           // 存储要删除第一个结点
    list->header = list->header->next; // 改变header指向为第一个结点的内存地址
  }
  else
  {
    Node *prev_node = find_prev_node(list, index); // 找当前位置的前一个结点
    del_node = prev_node->next;                    // 先把当前位置的结点存储起来
    prev_node->next = prev_node->next->next;       // 改变前一个结点中指针的指向为当前位置下一个结点的内存地址
  }
  // 先存储要删除位置的数据(从要删除的结点中取出来)
  *del_element = del_node->data;
  free(del_node);  // 释放当前要删除的结点
  del_node = NULL; // 重置为空指针
  list->length--;  // 长度减1
  return true;     // 删除成功
}
// 从尾部位置删除数据的函数
bool delete_end(LinkedList *list, int *del_element)
{
  return delete_at(list, list->length - 1, del_element);
}
// 获取指定位置的数据的函数
void get_value(LinkedList *list, size_t index, int *element)
{
  // 判断位置是否超出范围
  if (index >= list->length)
    return;
  if (index != 0)
  {
    Node *prev_node = find_prev_node(list, index); // 获取前一个结点
    *element = prev_node->next->data;              // 获取当前位置结点中的数据
    return;
  }
  *element = list->header->data; // 获取第一个结点中的数据
}
// 修改指定位置的数据的函数
bool edit_value(LinkedList *list, size_t index, int element)
{
  // 判断位置是否超出范围
  if (index >= list->length)
    return false;
  if (index != 0)
  {
    Node *prev_node = find_prev_node(list, index); // 获取前一个结点
    prev_node->next->data = element;               // 修改当前位置的数据
    return true;
  }
  list->header->data = element; // 修改的是第一个结点中的数据
  return true;
}
// 遍历链表
void printf_linked_list(LinkedList *list)
{
  if (list->length == 0)
    return;
  Node *current_node = list->header; // 获取第一个结点
  while (current_node != NULL)
  {
    printf("%d ", current_node->data);
    current_node = current_node->next; // 获取下一个位置的结点
  }
  printf("\n");
}
int main()
{
  LinkedList list;
  init_linked_list(&list); // 初始化链表
  insert_at(&list, 0, 500);
  insert_at(&list, 0, 400);
  insert_at(&list, 0, 300);
  insert_at(&list, 0, 200);
  insert_at(&list, 0, 100);
  printf("长度:%zu\n", get_length(&list));
  printf_linked_list(&list); // 遍历链表
  insert_end(&list, 600);    // 尾追
  printf("长度:%zu\n", get_length(&list));
  printf_linked_list(&list);  // 遍历链表
  edit_value(&list, 3, 1000); // 修改数据
  printf("长度:%zu\n", get_length(&list));
  printf_linked_list(&list); // 遍历链表
  int element = 0;
  get_value(&list, 3, &element); // 获取某个位置数据
  printf("数据是:%d\n", element);
  int del_element = 0;
  delete_at(&list, 3, &del_element); // 删除数据
  printf("删除的数据是:%d\n", del_element);
  printf("长度:%zu\n", get_length(&list));
  printf_linked_list(&list); // 遍历链表
  delete_end(&list, &del_element);
  printf("删除的数据是:%d\n", del_element);
  printf("长度:%zu\n", get_length(&list));
  printf_linked_list(&list);  // 遍历链表
  destroy_linked_list(&list); // 释放
  printf("长度:%zu\n", get_length(&list));
  printf_linked_list(&list); // 遍历链表
  return 0;
}