#include <stdio.h>
// 局部变量/局部常量/局部数组的使用范围在当前局部作用域中/函数中,还可以在块级作用域中使用
// 局部变量/局部常量/局部数组的生命周期从函数调用开始,到函数调用结束后销毁
// 形参也属于局部变量
// 局部变量/局部常量/局部数组声明未赋值,都是垃圾值
void f1(int a)
{
  // int num = 10;           // 局部变量
  // const double PI = 3.14; // 局部常量
  // char chs[] = "hello";   // 局部字符数组
  int num;           // 局部变量
  const double PI; // 局部常量
  char chs[500];   // 局部字符数组
  printf("f1函数内部,局部作用域...\n");
  printf("num=%d\n", num);
  printf("PI=%.2lf\n", PI);
  printf("chs=%s\n", chs);
  printf("a=%d\n", a);
  {
    // 块级作用域
    printf("块级作用域中...\n");
    printf("num=%d\n", num);
    printf("PI=%.2lf\n", PI);
    printf("chs=%s\n", chs);
     printf("a=%d\n", a);
  }
}
int main()
{
  printf("main函数\n");
  f1(10);
  int x;
  printf("x=%d\n",x);
  return 0;
}