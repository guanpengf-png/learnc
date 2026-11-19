#include <stdio.h>
int main()
{
  // （2）循环输出数字7~15
  for (int i = 7; i < 16; i++)
    printf("%d\n", i);
  printf("\n================\n");
  // （3）倒序输出数字56 ~ 43
  for (int i = 56; i > 42; i--)
    printf("%d\n", i);
  printf("\n================\n");
  // （4）输出10（包括10）以内所有的偶数
  for (int i = 0; i <= 10; i++)
    if (i % 2 == 0)
      printf("%d\n", i);
  printf("\n================\n");
  for (int i = 0; i <= 10; i += 2)
    printf("%d\n", i);
  printf("\n===============\n");
  // （5）计算100以内（包括100）所有数字的和
  int sum = 0; // 存储和的变量
  for (int i = 0; i <= 100; i++)
    sum += i;
  printf("和为:%d\n", sum);
  printf("\n======================\n");
  // （6）计算10以内所有奇数的乘积
  int ji = 1;
  for (int i = 0; i <= 10; i++)
    if (i % 2 != 0)
      ji *= i;
  printf("乘积结果:%d\n", ji);
  return 0;
}