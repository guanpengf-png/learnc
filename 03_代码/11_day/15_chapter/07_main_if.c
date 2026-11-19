#include <stdio.h>
// 条件编译指令:符合条件,则对应的代码会被编译,否则不编译
#define FLAG 0
// #if FLAG
// int num = 10;
// #endif

// #if FLAG
// int num = 10;
// #else
// int num = 20;
// #endif

#if FLAG==1
int num = 10;
#elif FLAG == 2
int num = 20;
#elif FLAG == 3
int num = 30;
#else
int num = 40;
#endif
int main()
{
  printf("num=%d\n",num);

  return 0;
}