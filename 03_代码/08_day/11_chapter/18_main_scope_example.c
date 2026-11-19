#include <stdio.h>
int len = 5; // 全局变量
int arr[5] = {10, 20, 30, 40, 50}; // 全局数组
void fn(int num)
{
  int a = 250; // 局部变量
  printf("%d \n", num + a); // 270
}
int main()
{
  fn(20);
  for (int i = 0; i < 5; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n============\n");
  fn(60);
  return 0;
}
