#include <stdio.h>
int main()
{
  double d1 = 3.987;
  double d2 = 4.235;
  int sum1 = d1 + d2;
  int sum2 = (int)d1 + (int)d2;
  int sum3 = (int)(d1 + d2); // 更合理一些
  printf("sum1的结果是:%d\n", sum1); // 8 
  printf("sum2的结果是:%d\n", sum2); // 7
  printf("sum3的结果是:%d\n", sum3); // 8

  int sum4 = (int)(2.5 * 10 + 2 * 1.5);
  printf("结果是:%d\n",sum4);

  return 0;
}