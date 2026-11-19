#include <stdio.h>
int main()
{
  int a = 10, b = 20, c = 30;
  int *ptr1 = &a;
  int *ptr2 = &b;
  int *ptr3 = &c;
  int *ptr_arr[] = {ptr1, ptr2, ptr3};       // 指针数组
  int len = sizeof(ptr_arr) / sizeof(int *); // 计算数组长度
  for (int i = 0; i < len; i++)
    printf("索引:%d,元素值:%p,指向的内存空间中的数据值是:%d===自己所在空间的内存地址:%p\n", i, ptr_arr[i], *ptr_arr[i], &ptr_arr[i]);

  // int len1 = sizeof(ptr_arr) / sizeof(ptr_arr[0]); // 计算数组长度
  // int len2 = sizeof(ptr_arr) / sizeof(int *); // 计算数组长度
  // printf("len1=%d,len2=%d\n",len1,len2);
  return 0;
}