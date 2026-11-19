#include <stdio.h>
// 1. 使用#define宏定义的方式来定义布尔类型及布尔值
#define BOOL int
#define TRUE 1
#define FALSE 0
int main()
{
  // 布尔类型,也是一种数据类型,c89是没有的,c99之后出现的
  // 布尔类型的数据值只有两个,真-----1-----true,假----0----false
  // 三种方式声明或使用布尔类型
  // 1. 使用#define宏定义的方式来定义布尔类型及布尔值
  // 2. 使用c99中提供的写法,_Bool来声明变量,使用0和非0的数据值表达布尔值
  // 3. 使用c99中提供的bool来声明变量,使用该true和false数据值表达布尔值

  // 声明布尔类型的变量并赋值
  BOOL flag1 = TRUE;
  BOOL flag2 = FALSE;
  printf("flag1=%d\n", flag1);
  printf("flag2=%d\n", flag2);

  return 0;
}