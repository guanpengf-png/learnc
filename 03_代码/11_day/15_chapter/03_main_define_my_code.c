#include <stdio.h>
#define 整型 int
#define 变量1 num1
#define 变量2 num2
#define 如果 if
#define 否则 else
#define 输出 printf

int main()
{
  整型 变量1 = 10;
  整型 变量2 = 20;
  如果(变量1 >变量2)
  输出("%d\n", 变量1);
  否则
  输出("%d\n", 变量2);

  return 0;
}