#include <stdio.h>
int main()
{
  // 指针变量可以访问/修改变量中的数据
  // 指针的作用:多了一种访问/修改数据的方式
  // 指针变量的指向是可以修改的
  int num = 10;
  int *ptr = &num;
  printf("num变量的值:%d,num变量的内存地址:%p\n", num, &num);
  printf("ptr指针变量指向的内存地址:%p\n", ptr);
  printf("ptr指针变量指向的内存地址对应的内存空间中的数据:%d\n", *ptr);
  printf("\n==================\n");
  // 指针变量修改变量的数据
  *ptr = 100;
  printf("num变量的值:%d,num变量的内存地址:%p\n", num, &num);
  printf("ptr指针变量指向的内存地址:%p\n", ptr);
  printf("ptr指针变量指向的内存地址对应的内存空间中的数据:%d\n", *ptr);
  printf("\n==================\n");
  // 指针变量的指向改变
  int num2 = 20;
  ptr = &num2;                                                          // 修改了指针变量的指向
  printf("num变量的值:%d,num变量的内存地址:%p\n", num, &num);           // 100 num的地址
  printf("num2变量的值:%d,num2变量的内存地址:%p\n", num2, &num2);       // 20 num2的地址
  printf("ptr指针变量指向的内存地址:%p\n", ptr);                        // num2的地址
  printf("ptr指针变量指向的内存地址对应的内存空间中的数据:%d\n", *ptr); // 20
  printf("\n==================\n");
  *ptr += 100;       // *ptr = *ptr+100;
  printf("num2变量的值:%d,num2变量的内存地址:%p\n", num2, &num2);       // 120 num2的地址
  printf("ptr指针变量指向的内存地址:%p\n", ptr);                        // num2的地址
  printf("ptr指针变量指向的内存地址对应的内存空间中的数据:%d\n", *ptr); // 120

  return 0;
}