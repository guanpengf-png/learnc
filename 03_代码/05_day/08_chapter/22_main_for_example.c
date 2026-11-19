#include <stdio.h>
int main()
{
  // 第一种形式:
  for (int i = 1; i <= 9; i++) // 控制的是行数
  {
    for (int j = 1; j <= i; j++) // 控制的是列数
      printf("%d*%d=%d\t", j, i, i * j);
    printf("\n");
  }
  printf("\n================\n");
  // 第二种形式:
  for (int i = 9; i >= 1; i--) // 控制的是行数
  {
    for (int j = 1; j <= i; j++) // 控制的是列数
      printf("%d*%d=%d\t", j, i, i * j);
    printf("\n");
  }
  printf("\n==================\n");
  // 第三种形式:
  for (int i = 1; i <= 9; i++) // 控制的是行数
  {
    for (int j = 1; j <= 9 - i; j++) // 随着行数的增加,\t的数量越来越少
      printf("\t");
    for (int j = 1; j <= i; j++) // 控制的是列数
      printf("%d*%d=%d\t", j, i, i * j);
    printf("\n");
  }
  printf("\n================\n");
  // 第四种形式:
  for (int i = 9; i >= 1; i--) // 控制的是行数
  {
    for (int j = 1; j <= 9 - i; j++) // 随着行数的增加,\t的数量越来越多
      printf("\t");
    for (int j = 1; j <= i; j++) // 控制的是列数
      printf("%d*%d=%d\t", j, i, i * j);
    printf("\n");
  }

  return 0;
}