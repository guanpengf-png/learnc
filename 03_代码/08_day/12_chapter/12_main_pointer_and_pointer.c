#include <stdio.h>
int main()
{
  // 数组名指向的是数组中第一个元素的内存地址(指向的是数组中首元素的地址)
  // 数组不是指针,指向不可改变
  // sizeof 数组名  计算的是数组的总字节数
  // sizeof 指针变量 计算的指向的内存地址所占的字节数:8个

  int nums[] = {10, 20, 30, 40, 50};
  // nums = {100,200,300};  
  printf("nums指向的内存地址:%p,对应的数据:%d\n", nums, *nums);
  for (int i = 0; i < 5; i++)
    printf("索引:%d,元素值:%d,内存地址:%p\n", i, nums[i], &nums[i]);
  printf("\n============\n");
  int *ptr = nums;
  printf("ptr指针变量所占的字节数:%zu\n",sizeof(ptr)); // 8
  printf("nums所占的字节数:%zu\n",sizeof(nums)); // 20
  // ptr是指针变量,指向可以改变
  // int *ptr = nums; // int *ptr = &nums[0]  等价关系
  // ptr + 1; // 可以
  // ptr++; // 可以
  // ptr += 1; // 可以
  // ptr = ptr + 1; 
  // &nums[0]指向的是内存地址,但是它不是指针变量,所以不能改变指向
  // nums是数组名,指向的是一个元素的内存地址,不是指针,所以,也不能改变指向
  // &nums[0] + 1; // 可以
  // &nums[0] = &nums[0] + 1; 不可以
  // &nums[0] += 1;不可以
  // &nums[0]++;不可以
  // nums + 1; // 可以
  // nums = nums + 1;不可以
  // nums += 1;不可以
  // nums++;不可以

  return 0;
}