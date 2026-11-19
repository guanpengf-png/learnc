#include <stdio.h>
int main()
{
  // 数组长度计算及遍历
  int nums[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  printf("数组的总字节数:%zu\n", sizeof(nums));        // 总字节数
  printf("第一个元素的字节数:%zu\n", sizeof(nums[0])); // 元素字节数
  // 数组长度 = 总字节数/单个元素的字节数
  //  int len = sizeof(nums) / sizeof(nums[0]); // 长度计算
  int len = sizeof(nums) / sizeof(int); // 长度计算
  //  printf("长度:%d\n",len);
  for (int i = 0; i < len; i++)
    printf("索引:%d,元素值:%d\n", i, nums[i]);

  return 0;
}