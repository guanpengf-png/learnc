#include <stdio.h>
// 带参数的宏定义
#define GET_MAX(a, b) a > b ? a : b
int get_max(int x, int y)
{
  return x > y ? x : y;
}
int main()
{

  int x = 10, y = 20;
  int max_num = GET_MAX(x, y);
  printf("结果是:%d\n", max_num);
  double max_num2 = GET_MAX(12.34, 34.56);
  printf("结果:%.2lf\n", max_num2);
  return 0;
}