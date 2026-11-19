#include <stdio.h>
#include <limits.h>
double get_array_sum(int *ptr, int len, int *max_num)
{
  // ptr---是指针变量,指向的是数组名,指向的是数组中第一个元素的内存地址
  // ptr====nums等价关系  nums[索引]  ptr[索引]
  double sum = 0;

  for (int i = 0; i < len; i++)
  {
    // sum += ptr[i];
    sum += *(ptr + i);
    // 判断
    if (*max_num < ptr[i])
      *max_num = ptr[i]; // 存储的是最大值
  }
  return sum / len;
}
int main()
{
  // 需求:通过函数实现,计算数组中所有数据的平均值和最大值
  int nums[] = {10, 20, 30, 40, 51};
  int len = sizeof(nums) / sizeof(int); // 计算数组长度
  // int max_number = INT_MIN;             // 用来存储最大值的变量
  int max_number = nums[0];             // 用来存储最大值的变量
  double avg = get_array_sum(nums, len, &max_number);
  printf("平均值为:%.2lf\n", avg);
  printf("数组中的最大值是:%d\n",max_number);

  return 0;
}