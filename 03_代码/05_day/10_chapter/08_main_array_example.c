#include <stdio.h>
// #include <limits.h>
int main()
{
  // 计算数组中所有元素的和及平均值,求最大值和对应索引,求最小值和对应索引
  int nums[] = {10, 20, 100, 30, 5, 40, 50}; // 255/7
  int sum = 0;  // 存储和的变量
  int len = sizeof(nums) / sizeof(int); // 计算数组长度
  int max_num = nums[0];  // 假设数组中第一个元素是最大值
  // int max_num = INT_MIN;
  int max_index = 0;      // 假设索引0就是最大值的索引
  int min_num = nums[0];  // 假设数组中第一个元素是最小值
  int min_index = 0;      // 假设索引0就是最小值的索引
  for (int i = 0; i < len; i++){
    sum += nums[i]; // 求和
    if (max_num < nums[i]){
      max_num = nums[i]; // 存储最大值
      max_index = i;     // 存储最大值的索引
    }
    if (min_num > nums[i]){
      min_num = nums[i]; // 存储最小值
      min_index = i;     // 存储最小值的索引
    }
  }
  printf("和为:%d,平均值:%.2lf\n", sum, sum * 1.0 / len);
  printf("最大值是:%d,对应的索引:%d\n", max_num, max_index);
  printf("最小值是:%d,对应的索引:%d\n", min_num, min_index);

  return 0;
}