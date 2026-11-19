#include <stdio.h>
#include <stdbool.h>
int main()
{
  // break是一个关键字,可以用来跳出某个循环

  {
    for (int i = 0; i < 5; i++)
    {
      if (i == 3)
        break;
      printf("%d\n", i);
    }
  }
  printf("\n=================\n");

  // 例子:编写程序，要求输入一个数字，判断该数字是否是质数。
  // int prime_number = 0;
  // printf("请输入一个数字:\n");
  // scanf("%d", &prime_number);
  // bool is_prime = prime_number > 1 ? true : false; // 假设默认所有的数字都是质数
  // // 7  7%2   7%3 7%4 7%5 7%6
  // for (int i = 2; i < prime_number; i++)
  // {
  //   if (prime_number % i == 0)
  //   {
  //     is_prime = false; // 不是质数
  //     break;
  //   }
  // }
  // if (is_prime)
  // {
  //   printf("%d是质数\n", prime_number);
  // }
  // else
  // {
  //   printf("%d不是质数\n", prime_number);
  // }

  {
    int prime_number = 0;
    printf("请输入一个数字:\n");
    scanf("%d", &prime_number);
    bool is_prime = prime_number > 1 ? true : false; // 假设默认所有的数字都是质数
    // 7  7%2   7%3 7%4 7%5 7%6
    for (int i = 2; i <= prime_number / 2; i++)
    {
      if (prime_number % i == 0)
      {
        is_prime = false; // 不是质数
        break;
      }
    }
    if (is_prime)
      printf("%d是质数\n", prime_number);
    else
      printf("%d不是质数\n", prime_number);
    // printf("%d%s\n",prime_number,is_prime?"是质数":"不是质数");
  }
  return 0;
}