#include <stdio.h>
// #include <stdbool.h>
int main()
{
   // 2. 使用c99中提供的写法,_Bool来声明变量,使用0和非0的数据值表达布尔值
   _Bool flag1 = 1; // 真
   _Bool flag2 = 0; // 假
   _Bool flag3 = -5; // 真
   printf("flag1=%d\n",flag1); // 1
   printf("flag2=%d\n",flag2); // 0
   printf("flag3=%d\n",flag3); // 1

   return 0;
}