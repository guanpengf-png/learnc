#include <stdio.h>
int *f1()
{
  int num = 10;
  return &num;
}
int main()
{
  // 野指针:
  /*
    1. 指针声明未赋值
    2. 指针指向了数组越界的位置
    3. 指针指向了已经释放的内存地址

     如何避免使用野指针?
    1. 指针声明要赋值
    2. 指针不要指向数组越界的位置
    3. 指针不要指向已经释放的内存地址
  */

  // 指针声明未赋值
  // int *ptr;
  // printf("ptr指向的地址:%p\n",ptr);
  // printf("ptr指向的地址对应的内存空间中的数据:%d\n",*ptr);
  // *ptr += 100;
  // printf("ptr指向的地址:%p\n",ptr);
  // printf("ptr指向的地址对应的内存空间中的数据:%d\n",*ptr);

  // 指针指向的了数组越界的位置
  // int nums[] = {10, 20, 30};
  // for (int i = 0; i < 3; i++)
  //   printf("索引:%d,元素值:%d,内存地址:%p\n", i, nums[i], nums + i);
  // int *ptr = &nums[2] + 1;
  // printf("ptr指向的地址:%p\n", ptr);
  // printf("ptr指向的地址对应的内存空间中的数据:%d\n", *ptr);

  // 指针指向了已经释放的内存地址
  int *ptr = f1();
  printf("ptr指向的地址:%p\n", ptr);
  printf("ptr指向的地址对应的内存空间中的数据:%d\n", *ptr);

  return 0;
}