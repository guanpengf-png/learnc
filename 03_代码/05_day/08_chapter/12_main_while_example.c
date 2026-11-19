#include <stdio.h>
int main()
{
  // 判断用户输入的密码是否正确,如果不正确,则提示继续输入密码,123
  // 循环次数不确定的循环
  int pwd = 0;
  while (pwd != 123)
  {
    printf("请您输入密码:\n");
    scanf("%d", &pwd); // 接收
  }
  printf("登录成功\n ");

  return 0;
}