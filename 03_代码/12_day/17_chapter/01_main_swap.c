#include <stdio.h>
// void swap(int x,int y){
//   printf("x=%d,y=%d\n",x,y);
//   int temp = x;
//   x = y;
//   y = temp;
//   printf("x=%d,y=%d\n",x,y);
// }
// 交换两个变量的值的函数
void swap(int *x, int *y)
{
  printf("x=%d,y=%d\n", *x, *y);
  int temp = *x;
  *x = *y;
  *y = temp;
  printf("x=%d,y=%d\n", *x, *y);
}

// void swap2(int *x, int *y)
// {
//   printf("x=%d,y=%d\n", *x, *y);
//   *x = *x + *y;
//   *y = *x - *y;
//   *x = *x - *y;
//   printf("x=%d,y=%d\n", *x, *y);
// }

void swap3(int *x, int *y)
{
  printf("x=%d,y=%d\n", *x, *y);
  *x = *x ^ *y;
  *y = *x ^ *y;
  *x = *x ^ *y;
  printf("x=%d,y=%d\n", *x, *y);
}
int main()
{
  // 需求:交换两个变量的值
  // int num1 = 10;
  // int num2 = 20;
  // printf("num1=%d,num2=%d\n", num1, num2);
  // swap(&num1, &num2);
  // printf("num1=%d,num2=%d\n", num1, num2);

  // printf("\n==============\n");
  // int num1 = 10;
  // int num2 = 20;
  // printf("num1=%d,num2=%d\n", num1, num2);
  // swap2(&num1, &num2);
  // printf("num1=%d,num2=%d\n", num1, num2);

   printf("\n==============\n");
  int num1 = 10;
  int num2 = 20;
  printf("num1=%d,num2=%d\n", num1, num2);
  swap3(&num1, &num2);
  printf("num1=%d,num2=%d\n", num1, num2);
  return 0;
}