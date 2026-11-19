#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// 定义一个动态数组的结构体类型
typedef struct
{
  int *data;       // 用来指向分配的内存空间的内存地址的指针
  size_t capacity; // 容量
  size_t length;   // 真实数据的个数(数组长度)
} DynamicArray;
// 初始化动态数组,通过容量分配内存空间,及初始化动态数组中所有成员的初始值的函数
void init_dynamic_array(DynamicArray *array, size_t init_capacity)
{
  array->data = (int *)malloc(sizeof(int) * init_capacity); // 分配内存并设置指针指向
  array->capacity = init_capacity;                          // 初始化容量
  array->length = 0;                                        // 初始化长度为0
}
// 扩容,重新分配内存空间的函数
void resize_dynamic_array(DynamicArray *array, size_t new_capacity)
{
  array->data = (int *)realloc(array->data, sizeof(int) * new_capacity); // 重新分配内存
  array->capacity = new_capacity;                                        // 更新容量
}
// 释放内存的函数
void destroy_dynamic_array(DynamicArray *array)
{
  free(array->data);   // 释放内存
  array->data = NULL;  // 重置为空指针
  array->capacity = 0; // 重置容量为0
  array->length = 0;   // 重置长度为0
}
// 获取长度的函数
size_t get_length(DynamicArray *array)
{
  return array->length;
}
// 向指定索引位置插入数据的函数
void insert_at(DynamicArray *array, size_t index, int element)
{
  // 判断索引是否超出范围
  if (index > array->length)
    return;
  // 判断容量是否满了
  if (array->length == array->capacity)
    resize_dynamic_array(array, array->capacity * 2); // 扩容
  // 遍历的方式向后移动数据
  for (size_t i = array->length; i > index; i--)
    array->data[i] = array->data[i - 1]; // 不停的向后移动数据
  array->data[index] = element;          // 把新数据赋值到当前的索引位置
  array->length++;                       // 更新长度
}
// 向数组的尾部追加一个数据的函数
void insert_end(DynamicArray *array, int element)
{
  insert_at(array, array->length, element);
}
// 从指定索引位置删除一个数据的函数
bool delete_at(DynamicArray *array, size_t index, int *del_element)
{
  // 判断索引是否超出范围
  if (index >= array->length)
    return false; // 返回失败
  // 把要删除位置的数据存储起来
  *del_element = array->data[index];
  // 遍历的方式向前移动数据
  for (size_t i = index; i < array->length - 1; i++)
    array->data[i] = array->data[i + 1];
  array->length--; // 更新长度
  return true;     // 返回成功
}
// 从数组的尾部位置删除一个数据的函数
bool delete_end(DynamicArray *array, int *del_element)
{
  return delete_at(array, array->length - 1, del_element);
}
// 遍历数组的函数
void printf_dynamic_array(DynamicArray *array)
{
  if (array->length == 0)
    return;
  for (size_t i = 0; i < array->length; i++)
    printf("%d ", array->data[i]);
  printf("\n");
}
int main()
{
  // 测试
  DynamicArray array;            // 声明结构体变量
  init_dynamic_array(&array, 5); // 初始化动态数组
  insert_at(&array, 0, 600);
  insert_at(&array, 0, 500);
  insert_at(&array, 0, 400);
  insert_at(&array, 0, 300);
  insert_at(&array, 0, 200);
  insert_at(&array, 0, 100);
  printf("长度:%zu\n", get_length(&array)); // 长度
  printf_dynamic_array(&array);            // 遍历
  insert_end(&array, 700);                 // 尾插
  printf("长度:%zu\n", get_length(&array)); // 长度
  printf_dynamic_array(&array);            // 遍历
  int del_element = 0;
  delete_at(&array, 3, &del_element); // 删除指定位置数据
  printf("删除的数据是:%d\n", del_element);
  printf("长度:%zu\n", get_length(&array)); // 长度
  printf_dynamic_array(&array);            // 遍历
  delete_end(&array, &del_element);        // 尾删
  printf("长度:%zu\n", get_length(&array)); // 长度
  printf_dynamic_array(&array);            // 遍历
  destroy_dynamic_array(&array);           // 释放
  printf("长度:%zu\n", get_length(&array)); // 长度
  printf_dynamic_array(&array);            // 遍历

  return 0;
}