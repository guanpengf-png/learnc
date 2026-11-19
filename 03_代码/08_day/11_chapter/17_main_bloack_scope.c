#include <stdio.h>
int main()
{
  // 块级作用域
  // 块级变量/块级常量/块级数组的使用范围在当前块级作用域中,生命周期从{}开始到{}结束
  // 块级变量/块级常量/块级数组声明未赋值还是垃圾值
  {
    int num;         // 块级变量
    const double PI; // 块级常量
    char chs[100];   // 块级字符数组
    printf("num=%d\n", num);
    printf("PI=%.2lf\n", PI);
    printf("chs=%s\n", chs);
  }

  return 0;
}