#include <stdio.h>
void f1(int *ptr, int len)
{
  for (int i = 0; i < len; i++)
    ptr[i] += 10;
}

int main()
{
  int nums[] = {10, 20, 30, 40, 50};
  for (int i = 0; i < 5; i++)
    printf("索引:%d,元素值%d\n", i, nums[i]);
  printf("\n================\n");
  f1(nums, 5);
  for (int i = 0; i < 5; i++)
    printf("索引:%d,元素值%d\n", i, nums[i]);

  return 0;
}