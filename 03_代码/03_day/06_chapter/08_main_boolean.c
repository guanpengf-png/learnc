#include <stdio.h>
#include <stdbool.h>
int main()
{
  // 3. 使用c99中提供的bool来声明变量,使用该true和false数据值表达布尔值
  bool flag1 = true;
  bool flag2 = false;
  printf("flag1=%d,flag2=%d\n", flag1, flag2);

  if (flag1){
    printf("为真\n");
  }else{
    printf("为假\n");
  }

  return 0;
}