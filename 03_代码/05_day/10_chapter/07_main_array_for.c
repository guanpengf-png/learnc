#include <stdio.h>
int main()
{
  // 数组中的元素正序和倒序遍历数组

  int nums[] = {10, 20, 30, 40, 50};
  int len = sizeof(nums) / sizeof(int); // 计算数组长度
  // 正序输出
  for (int i = 0; i < len; i++)
    printf("索引:%d,元素值:%d\n", i, nums[i]);
  printf("\n==========\n");
  // 倒序输出
  for (int i = len - 1; i >= 0; i--)
    printf("索引:%d,元素值:%d\n", i, nums[i]);

  return 0;
}