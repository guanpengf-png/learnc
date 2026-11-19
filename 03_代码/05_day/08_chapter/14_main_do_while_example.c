#include <stdio.h>
int main()
{
  // （2）循环输出数字7~15
  {
    int i = 7;
    do{
      printf("%d\n", i);
      i++;
    } while (i < 16);
    printf("最终i的值是:%d\n", i); }
  printf("\n====================\n");
  // （3）倒序输出数字53 ~ 48
  {
    int i = 53;
    do{
      printf("%d\n", i);
      i--;
    } while (i > 47);}
  printf("\n==================\n");
  // （4）输出10（包括10）以内所有的偶数
  {
    int i = 0;
    do {
      if (i % 2 == 0)
        printf("%d\n", i);
      i++;
    } while (i <= 10);}
  printf("\n================\n");
  // （5）计算100以内（包括100）所有数字的和
  {
    int i = 0, sum = 0;
    do{
      sum += i;
      i++;
    } while (i <= 100);
    printf("最终i的值是:%d,sum=%d\n", i, sum);}
  printf("\n=================\n");
  // （6）计算10以内所有奇数的乘积
  {
    int i = 0, ji = 1;
    do{
      if (i % 2 != 0)
        ji *= i;
      i++;
    } while (i <= 10);
    printf("%d\n",ji); }

  return 0;
}