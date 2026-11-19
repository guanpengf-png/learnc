#include <stdio.h>
int main()
{
  // 冗余
  // 编写程序，输入月份，输出该月份有多少天。说明：1月、3月、5月、7月、8月、10月、12月有31天，4月、6月、9月、11月有30天，2月有28 天或 29天。

  int month = 1; // 声明一个int类型的变量,起名为month,默认赋值为1
  printf("请您输入月份1-12:\n"); // 提示用户输入内容
  scanf("%d",&month); // 接收数据,并存储到month变量
  switch(month){
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:printf("%d月有%d天\n",month,31);break;

    case 4:
    case 6:
    case 9:
    case 11:printf("%d月有%d天\n",month,30);break;

    case 2:printf("%d有28或29天\n",month);break;
    default: printf("输入有误\n");break;
  }
  printf("程序结束\n");

  return 0;
}