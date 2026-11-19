#include <stdio.h>
// 函数指针:就是指针(指针变量),指向的是函数的内存地址
// 函数指针的声明或定义语法:1.先声明后赋值,2.声明的同时直接赋值
// 先声明后赋值的语法:
// 返回值类型名 (*函数指针名)([参数列表]);
// 函数指针 = 函数的内存地址;
// 声明的同时赋值的语法:
// 返回值类型名 (*函数指针名)([参数列表])= 函数的内存地址
// 函数指针的作用:多了一种调用函数的方式

// 函数的内存地址获取方式: &函数名   或者  函数名
// 求和
int get_sum(int x, int y){
  return x + y;
}
// 求差
int get_sub(int x, int y){
  return x - y;
}
int main()
{
  // 先声明一个函数指针,后赋值
  // int (*ptr)(int x, int y); // 声明一个函数指针,起名为ptr,类型是: int (*)(int x, int y)
  // ptr = &get_sum; // 设置ptr函数指针的指向为get_sum函数的内存地址
  // 声明函数指针的同时直接赋值
  // int (*ptr2)(int x, int y) = &get_sum;
  // printf("get_sum函数的内存地址是:%p\n",&get_sum);
  // printf("get_sum函数的内存地址是:%p\n",get_sum);

  // 声明函数指针的同时直接赋值
  int (*ptr1)(int x, int y) = &get_sum; // ptr1的类型是 int(*)(int,int)
  int (*ptr2)(int x, int y) = get_sum;
  int (*ptr3)(int, int) = &get_sum;
  int (*ptr4)(int, int) = get_sum;
  // 调用函数
  int sum1 = get_sum(10, 20); // 直接通过函数名来调用函数
  printf("和为:%d\n", sum1);
  int sum2 = (*ptr1)(100, 200); // 通过函数指针可以调用函数
  printf("和为:%d\n", sum2);
  int sum3 = ptr2(1000, 2000); // 通过函数指针可以调用函数
  printf("和为:%d\n", sum3);
  int sum4 = ptr3(11, 21); // 通过函数指针可以调用函数
  printf("和为:%d\n", sum4);
  int sum5 = ptr4(12, 22); // 通过函数指针可以调用函数
  printf("和为:%d\n", sum5);
  ptr4 = get_sub; // 改变函数指针的指向
  int sub = ptr4(100,20);
  printf("差为:%d\n",sub);

  return 0;
}