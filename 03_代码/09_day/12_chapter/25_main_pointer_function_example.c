#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *get_rand_array( int len)
{
  static int nums[10];             // 声明一个有限长度的空数组
  srand((unsigned int)time(NULL)); // 种子
  for (int i = 0; i < len; i++)
    nums[i] = rand() % 100; // 产生0-99之间任意一个
  return nums;
}
int main()
{
  // 编写一个函数，它会生成10个随机数，并使用数组名作为返回值。
  int *ptr = get_rand_array(10);
  for (int i = 0; i < 10;i++){
    printf("索引:%d,元素值:%d\n",i,ptr[i]);
  }

  return 0;
}