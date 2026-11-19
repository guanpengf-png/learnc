#include <stdio.h>
// void f1(int *ptr){
//   printf("数据是:%d\n",*ptr);
//   *ptr = 100;
//    printf("数据是:%d\n",*ptr);
// }

void f1(int num)
{
  printf("num=%d\n", num); // 10
  num += 100;
  printf("num=%d\n", num); // 110
}

void f2(int *ptr)
{
  printf("*ptr=%d\n", *ptr); // 10
  *ptr += 100;
  printf("*ptr=%d\n", *ptr); // 110
}
int main()
{
  int x = 10;
  printf("x=%d\n", x); // 10
  f2(&x);
  printf("x=%d\n", x); // 110

  // int x = 10;
  // printf("x=%d\n", x); // 10
  // f1(x);
  // printf("x=%d\n", x); // 10

  // int num = 10;
  // int *ptr = &num;
  // f1(&num);
  // f1(ptr);
  // printf("num=%d\n",num);

  // 指针是可以作为函数的参数使用
  // 通过把指针作为函数的参数使用,函数内部通过这个指针也可以修改该函数外部其他函数内部的数据值
  // 值传递:把数据值作为实参传递给形参,内部修改数据,外部数据不变
  // 引用(地址)传递:把内存地址作为实参传递给形参,内部修改数据,外部数据会改变

  return 0;
}