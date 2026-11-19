#include <stdio.h>
int main()
{
  // 判断用户输入的密码是否正确,如果不正确,则提示继续输入密码,123
  // 循环次数不确定的循环
  for (int pwd = 0; pwd != 123;){
    printf("请您输入密码:\n");
    scanf("%d",&pwd);
  }
  printf("登录成功\n");
  printf("\n===============\n");
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

  return 0;
}