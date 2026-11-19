#include <stdio.h>
int main()
{
  // 数组名指向的内存地址就是第一个元素的内存地址
  // int nums[] = {10, 20, 30, 40, 50};
  // printf("nums=%p\n",nums);
  // printf("nums数组名指向的内存地址:%p,对应内存空间中的数据是:%d\n",nums,*nums);
  // printf("\n=========================\n");
  // int *ptr = &nums[0];                  // ptr指针变量指向第一个元素的内存地址
  // int len = sizeof(nums) / sizeof(int); // 数组长度计算
  // printf("ptr指针变量指向的内存地址:%p,及对应内存空间中的数据:%d\n", ptr, *ptr);
  // for (int i = 0; i < len; i++)
  // {
  //   printf("索引:%d,元素值:%d,内存地址:%p\n", i, nums[i], &nums[i]);
  // }



  int nums[] = {10, 20, 30, 40, 50};
  // int *ptr = &nums[0];  // ptr指针变量指向第一个元素的内存地址
  int *ptr = nums;  // ptr指针变量指向第一个元素的内存地址
  int len = sizeof(nums) / sizeof(int); // 数组长度计算
  printf("ptr指针变量指向的内存地址:%p,及对应内存空间中的数据:%d\n", ptr, *ptr);
  for (int i = 0; i < len; i++)
  {
    
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, nums[i], &nums[i]);
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, i[nums], &i[nums]);
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, ptr[i], &ptr[i]);
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, i[ptr], &i[ptr]);
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, *(ptr+i),ptr+i);
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, *(nums+i),nums+i);
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, *(i+nums),i+nums);
    // printf("索引:%d,元素值:%d,内存地址:%p\n", i, *(i+ptr),i+ptr);
    
  }

  return 0;
}