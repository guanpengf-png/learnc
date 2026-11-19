#include <stdio.h>
// 宏定义
// #define 宏名 文本
// 1. 可以使用宏定义进行常量的声明,不要在结尾处使用分号,在预处理期间进行文本替换操作
#define PI 3.14
// 2. 可以使用宏定义进行数据类型的声明
#define BOOL int
#define TRUE 1
#define FALSE 0
// 3. 可以使用宏定义进行文本替换
#define PRINTF printf
// 4. 使用宏定义进行嵌套文本替换
// #define SUM a+b
// #define PRINTF2 printf("输出结果是:%d\n",SUM)
// 5. 宏定义的嵌套使用
#define N (y*5)
#define M (x*2+x*10)
#define RESULT M*N
int main()
{
  // double r = 5.0; // 半径
  // double s = PI * r * r;
  // printf("结果:%.2lf\n",s);
  // BOOL flag = TRUE;
  // PRINTF("今天真开心\n");
  // int a = 10, b = 20;
  int x = 10, y = 20;
  printf("%d\n",RESULT);
  // PRINTF2;

  return 0;
}