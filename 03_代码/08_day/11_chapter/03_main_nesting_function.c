#include <stdio.h>
// 不要嵌套声明,不是c的标准语法
void f1(){
  printf("f1函数\n");
  void f2(){
    printf("f2函数\n");
  }
  f2();
}
int main()
{
  // 函数声明或定义一个注意问题:函数不要嵌套声明
  f1();

  return 0;
}