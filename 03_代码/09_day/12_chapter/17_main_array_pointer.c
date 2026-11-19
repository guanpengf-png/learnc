#include <stdio.h>
int main()
{
  int nums[3] = {10, 20, 30};
  // nums是数组,不是指针,指向不可修改
  // nums = {100,200,300}; // 数组重新赋值

  int(*ptr)[3] = &nums; // ptr是数组指针,指向了数组的内存地址
  for (int i = 0; i < 3; i++)
    printf("索引:%d,元素值%d,对应的内存地址:%p\n", i, *(*ptr + i), *ptr + i);
  printf("\n==========\n");
  // 改变了数组指针的指向
  ptr = (int[3]){100, 200, 300};
  for (int i = 0; i < 3; i++)
    printf("索引:%d,元素值%d,对应的内存地址:%p\n", i, *(*ptr + i), *ptr + i);

  return 0;
}