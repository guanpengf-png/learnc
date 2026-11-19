#include <stdio.h>
// 全局作用域
// 全局变量/常量/数组的使用范围在整个.c文件中
// 全局变量/常量/数组声明未赋值,默认都是0
// 全局变量/常量/数组的生命周期从程序运行开始到程序运行结束
int num = 10;           // 全局变量
const double PI = 3.14; // 全局常量
char chs[] = "hello";   // 全局字符数组

// int num;           // 全局变量
// const double PI; // 全局常量
// char chs[1000];   // 全局字符数组

int main()
{
  // 局部作用域
  printf("main函数/局部作用域中...\n");
  printf("num=%d\n", num);
  printf("PI=%.2lf\n", PI);
  printf("chs=%s\n", chs);
  {
    // 块级作用域
    printf("块级作用域中...\n");
    printf("num=%d\n", num);
    printf("PI=%.2lf\n", PI);
    printf("chs=%s\n", chs);
  }

  return 0;
}