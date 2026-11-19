#include <stdio.h>
int main()
{
  int nums[] = {10,20,30};
  int len = sizeof(nums)/sizeof(int); // 计算数组长度
  for (int i = 0; i < len;i++)
    printf("索引:%d,元素值:%d,内存地址:%p\n",i,nums[i],&nums[i]);
  printf("\n==========================\n");
  int *ptr1 = nums; // ptr1指针变量指向的是数组名(数组中第一个元素的内存地址)
  int *ptr2 = &nums[0]; // ptr2指针变量指向的是数组中第一个元素的内存地址
  int(*ptr3)[3] = &nums; // ptr3数组指针指向的是数组的内存地址
  printf("ptr1指向的内存地址:%p\n",ptr1);
  printf("ptr2指向的内存地址:%p\n",ptr2);
  printf("ptr3指向的内存地址:%p\n",ptr3);
  printf("ptr1+1的结果:%p\n",ptr1+1); // 第二个元素的内存地址
  printf("ptr2+1的结果:%p\n",ptr2+1); // 第二个元素的内存地址
  printf("ptr3+1的结果:%p\n",ptr3+1); // 越界位置后的内存地址

  return 0;
}