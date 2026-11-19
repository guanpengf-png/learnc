#include <stdio.h>
int main()
{
  // 类型转换:指的是某个数据的数据值可以转换成其他的数据类型的值
  // 类型转换分为:自动类型转换(隐式转换)和强制类型转换(显示转换)
  // 类型转换的时候如果是窄类型转宽类型,属于自动类型转换(隐式转换)
  // 如果是宽类型转窄类型,属于强制类型转换(显示转换)
  // 窄类型:所占小字节的数据
  // 宽类型:所占大字节的数据
  // 如果short和int比较,short占2个,属于窄类型,int占4个,属于宽类型
  // 如果整数和浮点数比较,整数属于窄类型,浮点数属于宽类型
  // 所谓的类型转换,大的范围应该都是属于"相同的类型"的转换才合理
  // 类型转换的时机:在计算(运算)或赋值的时候进行的


  // 1. 在计算的过程中发生了自动类型转换
  // short s = 12;
  // int num = 23;
  // // printf("所占的字节数:%zu\n",sizeof(s+num));
  // printf("结果是:%d\n", s + num); // 在计算的过程中发生的类型转换,而且是窄转宽

  // signed int num1 = -10;
  // unsigned int num2 = 5;
  // printf("结果:%d\n",num1+num2);

  // float f = 12.34f;
  // double d = 23.45;
  // printf("字节数:%zu\n",sizeof(f+d));
  // printf("结果是:%lf\n",f+d);

  // int num = 10;
  // double d = 12.34;
  // printf("字节数:%zu\n",sizeof(num+d));
  // printf("结果是:%lf\n",num+d);

  // long long ll = 12LL;
  // float f = 12.34f;
  // printf("所占字节数:%zu\n",sizeof(ll+f)); // 4 
  // printf("结果:%f\n",ll+f); // 24.34

  // 在赋值的时候进行的自动类型转换
  int num = 10;
  double d = num;
  printf("d的值是:%lf\n",d);

  double d2 = 12;
  printf("d2的值是:%lf\n",d2);

  // 赋值的时候宽类型转窄类型,建议使用强转的方式
  // 语法: 数据类型 变量名 = (数据类型)宽类型的数据值
  int num2 = (int)12.34;
  printf("num2的结果是:%d\n",num2);

  return 0;
}