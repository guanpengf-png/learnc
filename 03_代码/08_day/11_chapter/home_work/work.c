#include <stdio.h>
#include <stdbool.h>
// bool is_prime_number(int x)
// {
//   // 假设大于1的所有数字都是质数
//   bool flag = x > 1 ? true : false;
//   for (int i = 2; i <= x / 2; i++)
//   {
//     if (x % i == 0)
//     {
//       flag = false;
//       break;
//     }
//   }
//   return flag;
// }
// 计算质数
bool is_prime_number(int x)
{
  // 假设大于1的所有数字都是质数
  // bool flag = x > 1 ? true : false;
  if (x <= 1)
    return false;
  for (int i = 2; i <= x / 2; i++)
    if (x % i == 0)
      return false;
  return true;
}
// 计算天数 2026 3 2
int get_days(int year, int month, int day)
{
  int sum_days = day; // 先保存天数
  // 数组中保存每个月的天数
  int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (int i = 0; i < month - 1; i++)
    sum_days += days[i]; // 累计当前月份前面每个月的天数
  // 计算闰年
  if (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month > 2)
    sum_days += 1;
  return sum_days;
}
// 画实心星星
void show_star()
{

  for (int i = 1; i <= 5; i++) // 控制的是行
  {
    for (int j = 0; j < 5 - i; j++) // 控制的是每一行的空格数量
      printf(" ");
    for (int j = 1; j <= 2 * i - 1; j++) // 控制的是列
    {
      printf("*");
    }
    printf("\n");
  }
}
// 画空心星星
void show_star2(int rows)
{

  for (int i = 1; i <= rows; i++) // 控制的是行
  {
    for (int j = 0; j < rows - i; j++) // 控制的是每一行的空格数量
      printf(" ");
    for (int j = 1; j <= 2 * i - 1; j++) // 控制的是列
    {
      if (j == 1||j==2*i-1||i==rows)
      {
        printf("*");
      }
      else
      {
        printf(" ");
      }
    }
    printf("\n");
  }
}
int main()
{
  // （1）定义函数，判断一个数字是否是质数。
  // int num = 7;
  // bool result = is_prime_number(num);
  // if (result)
  //   printf("%d是质数\n", num);
  // else
  //   printf("%d不是质数\n", num);
  // （2）定义一个函数，实现年月日，是今年的第多少天
  // int year = 2028, month = 3, day = 2;
  // int days = get_days(year, month, day);
  // printf("%d年%d月%d日是今年的第%d天\n", year, month, day, days);
  // （3）找某个数据在数组中是否存在,存在则返回索引,否则返回-1=====作业
  // （4）函数实现实心三角形
  show_star();
  printf("\n=======================\n");
  // （5）函数实现空心三角形
  show_star2(10);

  return 0;
}