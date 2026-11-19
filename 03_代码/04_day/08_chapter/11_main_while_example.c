#include <stdio.h>
int main()
{
  // （2）循环输出数字 7~15
  {
    int i = 7;
    while (i <= 15)
    {
      printf("%d\n", i);
      i++;
    }
    printf("i最终是:%d\n", i);
  }

  printf("\n===============\n");

  // （3）倒序输出数字 53 ~ 48
  {
    int i = 53;
    while (i > 47)
    {
      printf("%d\n", i);
      i--;
    }
    printf("i最终是:%d\n", i);
  }
  printf("\n===============\n");
  // （4）输出10（包括10）以内所有的偶数
  {
    // int i = 0;
    // while (i <= 10)
    // {
    //   if (i % 2 == 0)
    //     printf("%d\n", i);
    //   i++;
    // }
    // printf("i最终是:%d\n", i);
    int i = 0;
    while (i <= 10)
    {
      printf("%d\n", i);
      i += 2;
    }
    printf("i最终是:%d\n", i);
  }
  printf("\n===================\n");
  {
    // （5）计算100以内（包括100）所有数字的和
    int i = 0;   // 计数器
    int sum = 0; // 求和的变量
    while (i <= 100)
    {
      sum += i;
      i++;
    }
    printf("和为:%d,最终的i:%d\n", sum, i);
  }

  printf("\n==============\n");
  // （6）计算10以内所有奇数的乘积

  int i = 0; // 计数器
  int ji = 1;
  while (i <= 10)
  {
    if (i % 2 != 0)
      ji *= i;
    i++;
  }
  printf("结果:%d,最终i的值:%d\n", ji, i);
  return 0;
}