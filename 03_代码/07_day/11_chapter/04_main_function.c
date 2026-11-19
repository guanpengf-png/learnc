#include <stdio.h>
// 函数的声明或定义
void f1(int x){
  printf("f1函数=====%d\n",x);
}
int main()
{
  int a = 10;
  f1(a); // 函数调用----使用函数---函数中的代码就会执行
  f1(100);
  f1(100);
  return 0;
}