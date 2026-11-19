#include <stdio.h>
int main()
{
  //  3.指针变量可以自增自减运算,其本质向后/向前移动一个单位,自身指向可以改变
  // 结论:利用指针的自增或自减,可以进行数组的遍历
  // int nums[] = {10, 20, 30, 40, 50};
  // // 声明一个int*类型的指针变量,起名为ptr,指向的是数组中第一个元素的内存地址
  // int *ptr = &nums[0];
  // int len = sizeof(nums) / sizeof(int); // 长度
  // for (int i = 0; i < len; i++)
  //   printf("索引:%d,元素值:%d,内存地址:%p\n", i, nums[i], &nums[i]);
  // printf("\n=====================\n");
  // printf("ptr指针变量指向的内存地址:%p,及对应内存空间中的数据:%d\n", ptr, *ptr);
  // ptr++; // 指针变量进行自增运算,ptr = ptr+1; 或者 ptr+=1; 向后移动1个单位(4个字节)
  // printf("ptr指针变量指向的内存地址:%p,及对应内存空间中的数据:%d\n", ptr, *ptr);
  // ptr--; // 指针变量进行自检运算,ptr=ptr-1;或者 ptr-=1;向前移动一个单位(4个字节)
  // printf("ptr指针变量指向的内存地址:%p,及对应内存空间中的数据:%d\n", ptr, *ptr);

  printf("\n=====================\n");
  int nums[] = {10, 20, 30, 40, 50};
  int len = sizeof(nums) / sizeof(int); // 长度
  // 声明一个int*类型的指针变量,起名为ptr,指向的是数组中第一个元素的内存地址
  int *ptr = &nums[0];
  printf("ptr指针变量指向的内存地址:%p,及对应内存空间中的数据:%d\n", ptr, *ptr);
  printf("\n=====================\n");
  for (int i = 0; i < len; i++) // 正序遍历
  {
    printf("索引:%d,元素值:%d,内存地址:%p\n", i, *ptr, ptr);
    ptr++;
  }
  // 越界位置及数据
  printf("ptr指针变量指向的内存地址:%p,及对应内存空间中的数据:%d\n", ptr, *ptr);
  for (int i = len - 1; i >= 0; i--) // 倒序遍历
  {
    ptr--;
    printf("索引:%d,元素值:%d,内存地址:%p\n", i, *ptr, ptr);
  }

  return 0;
}