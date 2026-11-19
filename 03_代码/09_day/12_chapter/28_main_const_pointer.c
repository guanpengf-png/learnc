#include <stdio.h>
int main()
{
  // const指针:就是指针,使用的是const关键进行修饰
  // const指针书写方式两种：无论是哪一种,const在*的左侧
  // 1. const 数据类型 * 指针名;
  // 2. 数据类型 const * 指针名;
  // const指针的特点:改变指向,但是不能通过解引用修改变量的值

  int num = 10;
  // const int *ptr = &num;
   int const *ptr = &num;
  int num2 = 20;
  printf("num的值是:%d,对应的内存地址是:%p\n",num,&num);
  printf("ptr指针指向的内存地址对应的空间中的数据是:%d\n",*ptr);
  printf("ptr指针指向的内存地址:%p\n",ptr);
  // *ptr = 100; // 该const指针不能解引用修改变量的数据
  ptr = &num2; // 该const指针的指向可以改变
  printf("num的值是:%d,对应的内存地址是:%p\n", num, &num);
  printf("num2的值是:%d,对应的内存地址是:%p\n", num2, &num2);
  printf("ptr指针指向的内存地址对应的空间中的数据是:%d\n",*ptr);
  printf("ptr指针指向的内存地址:%p\n",ptr);


  return 0;
}