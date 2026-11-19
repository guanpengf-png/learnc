#include <stdio.h>
int main()
{
  // （1）判断一个年份是否为闰年。
  // （2）判断一个整数是否是水仙花数，所谓水仙花数是指一个3位数，其各个位上数字立方和等于其本身，例如：153 = 1*1*1 + 5*5*5 + 3*3*3。

  // int h, t, s;
  // for (int i = 100; i < 1000; i++)
  // {
  //    h = i / 100;     // 百位
  //    t = i / 10 % 10; // 十位
  //    s = i % 10;      // 个位
  //   if (h * h * h + t * t * t + s * s * s == i)
  //     printf("%d ",i);
  // }

  // （3）编写程序，根据输入的月份和年份，求出该月的天数（1-12）, 就是需要考虑闰年（2月份 29）和平年（2月份 28）。
  // int year, month;
  // printf("输入年份和月份:\n");
  // scanf("%d %d", &year, &month);
  // switch (month)
  // {
  // case 1:
  // case 3:
  // case 5:
  // case 7:
  // case 8:
  // case 10:
  // case 12:
  //   printf("%d年%d月有%d天\n", year, month, 31);
  //   break;
  // case 4:
  // case 6:
  // case 9:
  // case 11:
  //   printf("%d年%d月有%d天\n", year, month, 30);
  //   break;

  // case 2:
  //   if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
  //     printf("%d年%d月有%d天\n", year, month, 29);
  //   else
  //     printf("%d年%d月有%d天\n", year, month, 28);
  //   break;
  // default:
  //   printf("输入有误\n");
  //   break;
  // }

  // （4）输出小写的a-z以及大写的Z—A。
  // for (int i = 0; i < 26; i++)
  //   printf("%c ", i + 97);
  // printf("\n");
  // for (int i = 0; i < 26; i++)
  //   printf("%c ", 90 - i);
  // （5）求出1-1/2+1/3-1/4…..1/100的和。
  double sum = 0; // 最终的和是浮点数
  for (int i = 1; i <= 100; i++)
  {
    if (i % 2 == 0) // 偶数
      sum -= 1.0 / i;
    else // 奇数
      sum += 1.0 / i;
  }
  printf("结果是:%.2lf\n", sum);

  return 0;
}