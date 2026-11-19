#include <stdio.h>
// 局部变量函数调用开始生命周期,调用结束,生命周期也结束(被销毁了)
// 静态局部变量从函数第一次调用,开始生命周期,函数调用结束后,不会被销毁,程序结束后才会被销毁
// 局部变量如果使用static修饰,会延长生命周期
// 静态局部变量声明未赋值,默认是0
void f1()
{
  // int a = 10; // 局部变量
  int a; // 局部变量
  // static int n = 10; // 静态局部变量
  static int n; // 静态局部变量
  printf("n=%d,a=%d\n", n, a);
  a++;
  n++;
  printf("n=%d,a=%d\n", n, a);
  printf("\n==============\n");
}
int main()
{
  f1();
  f1();
  f1();

  // static是一个关键字,表示静态的
  // 无论是变量,还是常量,还是数组,或者是函数,都可以使用static进行修饰,修饰后都属于静态
  // 所以,就有静态变量/静态常量/静态数组/静态函数的概念
  // static int num = 10;  // num就是静态变量
  // static const double PI = 3.14; // PI就是静态常量
  // static int nums[]={10,20,30}; // nums就是静态数组
  // static void f1(){} // f1就是静态函数

  return 0;
}