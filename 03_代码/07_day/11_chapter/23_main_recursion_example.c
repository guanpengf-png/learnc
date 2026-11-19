#include <stdio.h>
void f1(int x)
{
  if (x > 1)
    f1(x - 1);
  printf("%d\n", x); // 3---挂起
}

void f2(int x){
  if(x<3)
    f2(x + 1);
  printf("%d\n",x);
}
int main()
{
  f1(3);
  printf("\n==========\n");
  f2(1);

  return 0;
}