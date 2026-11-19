#include <stdio.h>
// 定义一个共用体类型
union Data
{
  int n;   // 4个字节
  char c;  // 1个字节
  float f; // 4个字节
};
int main()
{
  // 声明共用体类型变量,并赋值
  // union Data dt;
  // dt.c = 'A';
  // printf("dt.c的结果是:%c\n", dt.c);
  // printf("dt.n的结果是:%c\n", dt.n);
  // printf("dt.f的结果是:%c\n", dt.f);
  // printf("dt的结果是:%c\n", dt);

  // union Data dt2 = {12.34, 120}; // 大括号的方式赋值,只能赋值一个数据值,后面的数据值没有意义
  // printf("dt2的值是:%d\n", dt2);
  // printf("dt2的值是:%d\n", dt2.n);
  // printf("dt2的值是:%d\n", dt2.c);
  // printf("dt2的值是:%d\n", dt2.f);
  // union Data dt3 = {.f = 12.34};
  // printf("dt3的值是:%f\n", dt3);
  // printf("dt3的值是:%f\n", dt3.n);
  // printf("dt3的值是:%f\n", dt3.c);
  // printf("dt3的值是:%f\n", dt3.f);
  // 定义一个共用体类型
  union DATA
  {
    double d;
    float f;
  };
  union DATA dt1 = {12.34,23.45f};
  printf("dt1的结果是:%lf\n",dt1);
  printf("dt1.d的结果是:%lf\n",dt1.d);
  printf("dt1.f的结果是:%f\n",dt1.f);

  union DATA dt2;
  dt2.f = 34.56f;
  printf("dt2.f的结果是:%f\n",dt2.f);

  return 0;
}