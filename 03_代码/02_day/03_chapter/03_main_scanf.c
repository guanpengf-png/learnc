#include <stdio.h>
int main()
{
  // 声明一个int类型的变量,用来存储用户输入的数据的
  // int num;
  // // 提示内容
  // printf("请您输入一个整数:\n");
  // // 用来接收用户输入内容的语句代码
  // scanf("%d",&num);
  // printf("num变量中存储的数据是:%d\n",num);

  // int num1, num2;
  // printf("请您输入两个整数,可以使用空格隔开:\n");
  // scanf("%d %d",&num1,&num2);
  // printf("num1的值是%d,num2的值是%d\n",num1,num2);

  int num1, num2;
  printf("请您输入两个整数,可以使用逗号隔开:\n");
  scanf("%d,%d", &num1, &num2);
  printf("num1的值是%d,num2的值是%d\n", num1, num2);

  return 0;
}