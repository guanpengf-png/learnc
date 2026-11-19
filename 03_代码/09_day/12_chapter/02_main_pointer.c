#include <stdio.h>
int main()
{
  // 指针变量的声明或定义的语法:
  // 1.指针变量先声明后赋值的语法: 数据类型名 * 指针变量名; 指针变量名 = &变量名;
  int num = 10; // 声明了一个变量,起名为num,类型是int
  int *ptr;     // 声明了一个指针变量,起名为ptr,类型是int*
  ptr = &num;   // 为指针变量赋值为num变量的内存地址, &----取址符
  printf("num=%d,num变量的内存地址:%p\n", num, &num);
  printf("ptr指针变量指向的内存地址对应的内存空间中的数据是:%d\n", *ptr); // *---解引用
  printf("ptr指针变量指向的内存地址:%p\n", ptr);
  printf("\n===================\n");
  // 2.指针变量声明的同时直接赋值的语法: 数据类型名 * 指针变量名= &变量名;
  double d_num = 12.34;  // 声明了一个变量,起名为d_num,类型是double
  double *ptr2 = &d_num; // 声明了一个指针变量,起名为ptr2,类型是double *,指向的是d_num变量的内存地址
  printf("d_num=%.2lf,d_num变量的内存地址:%p\n", d_num, &d_num);
  printf("ptr2指针变量指向的内存地址对应的内存空间中的数据是:%.2lf\n", *ptr2);
  printf("ptr2指针变量指向的内存地址:%p\n", ptr2);

  return 0;
}