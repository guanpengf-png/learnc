#include <stdio.h>
int f1(int x,int y){
  int sum = x + y;
}

int f2(int x,int y){
  int max_num = x > y ? x : y;
  return max_num;
}

int f3(double x,double y){
  return (int)(x + y);
}
double f4(int x,int y){
  return x - y;
}
int main()
{
  // int r1= f1(30, 20);
  // printf("r1=%d\n",r1);
  // int r3 = f3(12.34, 12.93);
  // printf("r3=%d\n",r3);
  double r4 = f4(100, 20);
  printf("r4=%.2lf\n",r4);

  return 0;
}