#include <stdio.h>
int main()
{
  //  5.指针变量可以比较运算/关系运算
  int nums[] = {10, 20, 30, 40, 50};
  int len = sizeof(nums) / sizeof(int); // 数组长度计算
  double d_num = 12.34;
  int *ptr1 = &nums[0];
  int *ptr2 = &nums[3];
  int *ptr3 = &nums[0];
  printf("\n=======================\n");
  for (int i = 0; i < len; i++)
    printf("索引:%d,元素值:%d,内存地址:%p\n", i, nums[i], &nums[i]);
  printf("\n=======================\n");
  printf("ptr1>ptr2的结果是:%d\n",ptr1>ptr2); // 0
  printf("ptr1<ptr2的结果是:%d\n",ptr1<ptr2); // 1
  printf("ptr1==ptr3的结果是:%d\n",ptr1==ptr3); // 1
  double *ptr4 = &d_num;
  printf("ptr1>ptr4的结果是:%d\n",ptr1>ptr4); // 1
  printf("ptr1指向的内存地址:%p\n",ptr1);
  printf("ptr4指向的内存地址:%p\n",ptr4);

  return 0;
}