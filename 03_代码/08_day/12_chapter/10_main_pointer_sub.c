#include <stdio.h>
int main()
{
  //  4.同类型指针变量可以进行相减运算:其结果是相差的元素数(n个字节)
  int nums[] = {10, 20, 30, 40, 50};
  int *ptr1 = &nums[4];                 // 第五个元素的内存地址
  int *ptr2 = &nums[2];                 // 第三个元素的内存地址
  int len = sizeof(nums) / sizeof(int); // 数组长度计算
  printf("ptr1指针变量指向的内存地址:%p\n", ptr1);
  printf("ptr2指针变量指向的内存地址:%p\n", ptr2);
  printf("\n=======================\n");
  for (int i = 0; i < len; i++)
    printf("索引:%d,元素值:%d,内存地址:%p\n", i, nums[i], &nums[i]);
  printf("\n=======================\n");
  printf("ptr1-ptr2=%td\n", ptr1 - ptr2);
  printf("ptr2-ptr1=%td\n", ptr2 - ptr1);
  printf("\n=======================\n");
  double d1 = 12.34;
  double d2 = 23.45;
  double *ptr3 = &d1;
  double *ptr4 = &d2;
  printf("ptr3指向的内存地址:%p\n",ptr3);
  printf("ptr4指向的内存地址:%p\n",ptr4);
  printf("ptr3-ptr4=%td\n",ptr3-ptr4);// 
  return 0;
}