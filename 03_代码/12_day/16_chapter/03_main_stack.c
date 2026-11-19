#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// 定义栈的结构体类型
typedef struct
{
  int *data;       // 指针
  size_t capacity; // 容量
  size_t length;   // 长度
} Stack;
// 初始化栈的函数
void init_stack(Stack *stack, size_t init_capacity)
{
  stack->data = (int *)malloc(sizeof(int) * init_capacity); // 分配空间设置指针指向
  stack->capacity = init_capacity;                          // 初始化容量
  stack->length = 0;                                        // 初始化长度
}
// 扩容的函数
void resize_stack(Stack *stack, size_t new_capacity)
{
  stack->data = (int *)realloc(stack->data, sizeof(int) * new_capacity); // 扩容同时重新设置指针指向
  stack->capacity = new_capacity;                                        // 更新容量
}
// 释放栈的函数
void destroy_stack(Stack *stack)
{
  free(stack->data);   // 释放
  stack->data = NULL;  // 重置为空指针
  stack->capacity = 0; // 重置容量为0
  stack->length = 0;   // 重置长度为0
}
// 获取长度函数
size_t get_length(Stack *stack)
{
  return stack->length;
}
// 入栈的函数
void push(Stack *stack, int element)
{
  // 判断容量是否满了
  if (stack->length == stack->capacity)
    resize_stack(stack, stack->capacity * 2); // 扩容
  // *(stack->data + stack->length) = element;
  stack->data[stack->length] = element; // 添加数据
  stack->length++;                      // 更新长度
}
// 出栈的函数
bool pop(Stack *stack, int *del_element)
{
  if (stack->length == 0) // 没有数据
    return false;
  *del_element = stack->data[stack->length - 1]; // 先保存要删除的数据
  stack->length--;                               // 更新长度减1
  return true;
}
// 遍历栈的函数
void printf_stack(Stack *stack)
{
  if (stack->length == 0)
    return;
  for (size_t i = 0; i < stack->length; i++)
    printf("%d ", stack->data[i]);
  printf("\n");
}
int main()
{
  Stack s;
  init_stack(&s, 5); // 初始化栈
  push(&s, 100);
  push(&s, 200);
  push(&s, 300);
  push(&s, 400);
  push(&s, 500);
  push(&s, 600);
  printf("长度:%zu\n", get_length(&s));
  printf_stack(&s); // 遍历
  int del_element = 0;
  pop(&s, &del_element); // 出栈
  printf("出栈的数据:%d\n", del_element);
  printf("长度:%zu\n", get_length(&s));
  printf_stack(&s);  // 遍历
  destroy_stack(&s); // 释放
  printf("长度:%zu\n", get_length(&s));
  printf_stack(&s); // 遍历
  return 0;
}