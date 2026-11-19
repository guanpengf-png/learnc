#include <stdio.h>
#define PI 3.14
void f1(){
  double r = 5.0;
  double s = PI * r * r;
  printf("结果:%.2lf\n",s);
}
// 取消宏定义
#undef PI
void f2(); // 函数原型声明
int main()
{
  f1();
  f2();

  return 0;
}
void f2(){
  //  printf("结果:%.2lf\n", PI * 5.0*5.0);
}