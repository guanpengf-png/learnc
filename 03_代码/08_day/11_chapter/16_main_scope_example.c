#include <stdio.h>
int a = 100;
int b = 200;
void show_ab()
{
  int a = 300;
  a += 10;
  b += 20;
  printf("函数内部a = %d ,b = %d \n", a, b); // 310 220
}
int main()
{
  show_ab();
  printf("函数外部a = %d ,b = %d \n", a, b); //   100 220 
  return 0;
}
