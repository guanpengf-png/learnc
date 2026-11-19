#include <stdio.h>
int main()
{
  // 判断用户输入的密码是否正确,如果不正确,则提示继续输入密码,123
  // 循环次数不确定的循环
  {
    int pwd = 0;
    do
    {
      printf("请您输入密码:\n");
      scanf("%d", &pwd);
    } while (pwd != 123);
    printf("登录成功\n");
  }

  printf("\n=====================\n");
  {
    int pwd = 0;
    while (pwd != 123)
    {
      printf("请您输入密码:\n");
      scanf("%d", &pwd); // 接收
    }
    printf("登录成功\n ");
  }
  // 总结:while循环先判断条件是否成立,再决定是否执行循环体语句
  // do-while循环先执行一次循环体语句,再判断循环条件是否成立
  // while循环是先判断后循环,do-while是先循环后判断
  // 前者有可能一次循环体语句都不执行,后者至少执行一次循环体语句
  return 0;
}