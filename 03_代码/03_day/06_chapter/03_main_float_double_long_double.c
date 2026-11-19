#include <stdio.h>
int main()
{
  {
    // 单精度浮点型 float
    // float类型的数据在赋值的时候使用的后缀是f/F
    // float类型的数据在输出的时候使用的格式占位符是%f
    // float类型的数据在输出的时候,默认会保留6位有效数字,还会进行四舍五入计算
    // %.数字f---表示保留几位有效小数,默认也会进行四舍五入计算
    // float f = 12.34f; // 声明一个float类型的变量,起名为f,并赋值12.34
    float f1 = 0.12345f;
    float f2 = 3.11592f;
    float f3 = -2e12f;
    float f4 = 1.9823e2f;
    printf("f1=%f,f2=%.2f,f3=%f,f4=%f\n", f1, f2, f3, f4);
    printf("f1=%e,f2=%.2e,f3=%e,f4=%e\n", f1, f2, f3, f4);
  }
  printf("\n==========================\n");
  {
    // 双精度浮点型 double
    // double d = 12.34;
    // double类型的数据输出使用的是%lf,默认也是保留6位小数,也会进行四舍五入计算
    // 无论是float还是double都可以使用%e格式占位符以科学计算法的方式展示数据
    double d1 = 3.1415926;
    double d2 = 0.123456;
    double d3 = -2e12;
    double d4 = 1.9823e2;
    printf("d1=%lf,d2=%lf,d3=%lf,d4=%lf\n", d1, d2, d3, d4);
    printf("d1=%e,d2=%.2e,d3=%e,d4=%e\n", d1, d2, d3, d4);
  }
  printf("\n==========================\n");
  {
    // 长双精度浮点型 long double
    // long double类型的数据后缀是l/L
    // 对应的格式占位符是%Lf,对应的科学计算法的占位符是%Le
    long double ld = 12.34L;
    printf("ld变量的值是:%.1Lf\n",ld);
    printf("ld变量的值是:%.1Le\n",ld);

   
  }

  return 0;
}