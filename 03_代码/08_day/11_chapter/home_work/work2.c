#include <stdio.h>
int get_index(int nums[], int len, int target)
{
  for (int i = 0; i < len; i++)
    if (target == nums[i]) // 目标数据和每个数据进行对比
      return i;
  return -1;
}
int main()
{
  // （3）找某个数据在数组中是否存在,存在则返回索引,否则返回-1=====作业
  int nums[] = {10, 20, 30, 40, 5, 50, 60, 70, 80, 90};
  int len = sizeof(nums) / sizeof(int); // 计算数组长度
  int target = 5;                       // 要查找的目标数据
  int index = get_index(nums, len, target);
  printf("索引:%d\n", index);
  return 0;
}