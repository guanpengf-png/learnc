#include <stdio.h>
// 标识符常量之使用const关键字来定义常量
// 语法:const 数据类型名 常量名 = 值;
// 使用const关键字定义常量书写代码的位置,可以在main的外部,也可以在main的内部

// 需求,通过标识符常量const关键字的方式,计算圆的面积

const double PI = 3.14; // 常量
int main()
{
  // PI = 23.45; // 报错的,不可以修改的
  double r = 5.0; // 半径
  double s = PI * r * r;
  printf("面积是:%.2lf\n", s);

  // printf("结果:%d\n",34*23);
  return 0;
}