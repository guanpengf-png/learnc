#include <stdio.h>
// 计算斐波纳切数列
int get_fib(int x){
  if(x==1||x==2)
    return 1;
  return get_fib(x - 1) + get_fib(x - 2);
}
int main()
{
  int result = get_fib(12);
  printf("结果:%d\n",result);

  return 0;
}