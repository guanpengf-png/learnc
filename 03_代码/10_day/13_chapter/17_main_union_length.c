#include <stdio.h>
// 定义共用体类型
union data{
  short s; // 2个字节
  char ch; // 1个字节
  int n; // 4个字节
};
int main()
{
  union data dt;
  dt.ch = 'A';
  printf("dt的值是:%c\n",dt);
  printf("dt变量所占的字节数:%zu\n",sizeof(dt));
  // 共用体变量所占的字节数是最长成员的字节数

  return 0;
}