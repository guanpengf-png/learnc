#include <stdio.h>
// extern关键字,作用:声明外部链接
extern int num1;         // 全局变量
extern const double PI1; // 全局常量
extern char chs1[6];     // 全局数组
extern void f1();        // 函数

extern int num2;         // 全局变量
extern const double PI2; // 全局常量
extern char chs2[6];     // 全局数组
extern void f2();        // 函数
int main()
{
  printf("main function\n");
  printf("num1 value is:%d\n", num1);
  printf("PI1 value is:%.2lf\n", PI1);
  printf("chs1 value is:%s\n", chs1);
  f1();

  printf("num2 value is:%d\n", num2);
  printf("PI2 value is:%.2lf\n", PI2);
  printf("chs2 value is:%s\n", chs2);
  f2();

  return 0;
}