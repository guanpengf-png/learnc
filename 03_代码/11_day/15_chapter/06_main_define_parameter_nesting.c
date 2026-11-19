#include <stdio.h>
// #define SQ(x) x*x
#define SQ(x) (x)*(x)

int get_sq(int a){
  return a * a;
}
int main()
{

  // int num = 10;
  // int result = SQ(num+10); // num+10*num+10
  // printf("结果:%d\n",result);
  int i = 0;
  while(i<5){
    // printf("结果是:%d\n",get_sq(i++));
    printf("结果是:%d\n",SQ(i++)); // (i++)*(i++)
  }

  return 0;
}