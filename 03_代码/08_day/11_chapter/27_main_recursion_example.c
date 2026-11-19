#include <stdio.h>
int get_factorial(int x){
  if(x==1)
    return 1;
  return x * get_factorial(x - 1);
}
int main()
{
  // 求8的阶乘
  int result = get_factorial(8);
  printf("结果:%d\n",result);
  return 0;
}