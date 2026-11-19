#include <stdio.h>
int main()
{
  // 声明一个数组指针,指向某个数组的内存地址
  int nums[] = {10,20,30,40,50}; // 数组,类型是int[]
  int(*arr_ptr)[5] = &nums; // 数组指针,起名为arr_ptr,类型是: int(*)[5]
  // arr_ptr -----&nums
  // *arr_ptr------nums
  printf("nums数组的内存地址是:%p\n",&nums);
  printf("数组指针arr_ptr指向的内存地址:%p\n",arr_ptr);
  // 向后移动一个单位:20个字节,自身指向不变
  printf("arr_ptr+1的结果是:%p\n",arr_ptr+1);
  // 向后移动一个单位:20个字节,自身指向不变
  printf("&nums+1的结果是:%p\n",&nums+1);
  // &nums + 1; // &nums+=1;不可以 &nums++;不可以 &nums=&nums+1;
  int len = sizeof(nums) / sizeof(int);
  for (int i = 0; i < len  ;i++){
    // printf("索引:%d,元素值%d,对应的内存地址:%p\n",i,(*arr_ptr)[i],&(*arr_ptr)[i]);
    printf("索引:%d,元素值%d,对应的内存地址:%p\n",i,*(*arr_ptr+i),*arr_ptr+i);
  }
  printf("\n=======================\n");
  arr_ptr += 2;

  return 0;
}