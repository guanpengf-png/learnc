#include <stdio.h>
void f1(int x){
  printf("%d\n",x);
  if(x>1)
    f1(x - 1);
  printf("%d\n",x);
}
int main()
{
  f1(3); // 3 2 1 1 2 3

  return 0;
}