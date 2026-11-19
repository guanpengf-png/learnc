#include <stdio.h>
// static可以用来声明静态变量/常量/数组/函数
// static可以延长局部变量的生命周期,在全局静态区/常量区
// extern关键字,作用:声明外部链接,就是让其他文件中的数据在某个文件中使用,但是不安全
// 在多文件进行编译的时候,如果有些数据不想在其他的文件中使用,可以使用static关键字进行修饰,其他文件中即使使用了extern关键字进行声明外部链接操作,也是无法使用static修饰的数据,起到了安全保护作用


int num1 = 10; // 全局变量
const double PI1 = 3.14; // 全局常量
char chs1[6] = "hello"; // 全局数组
void f1(){
  printf("f1 function\n");
}

static int num2 = 10; // 静态全局变量
static const double PI2 = 3.14; // 静态全局常量
static char chs2[6] = "hello"; // 静态全局数组
// 静态函数
static void f2(){
  printf("f1 function\n");
}