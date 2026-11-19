#include <stdio.h>
// 交换两个变量值的函数
void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}
// 冒泡排序的函数
void bubble_sort(int *arr, int len)
{
  for (int i = 0; i < len - 1; i++)       // 控制的是比较的轮数
    for (int j = 0; j < len - 1 - i; j++) // 每一轮比较的次数
      if (arr[j] > arr[j + 1])            // 对比
        swap(&arr[j], &arr[j + 1]);       // 交换
}
// 遍历数组的函数
void printf_array(int *arr, int len)
{
  for (int i = 0; i < len; i++)
    printf("%d ", arr[i]);
  printf("\n");
}
int main()
{
  int nums[] = {30, 50, 20, 90, 10, 80, 60, 70}; // 数组
  int len = sizeof(nums) / sizeof(int);          // 计算数组长度
  bubble_sort(nums, len);                        // 排序的函数
  printf_array(nums, len);                       // 遍历数组的函数

  return 0;
}