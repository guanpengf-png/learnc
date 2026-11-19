#include <stdio.h>
// 二分查找法
int binary_search(int *arr, int len, int target)
{
  // 开始索引,结束索引,中间位置索引
  int start = 0, end = len - 1, mid = 0;
  while (start <= end)
  {
    mid = (start + end) / 2; // 中间索引
    if (target > arr[mid])
      start = mid + 1; // 修改开始索引
    else if (target < arr[mid])
      end = mid - 1; // 修改结束索引
    else
      return mid; // 找到了则返回索引
  }
  return -1;
}
// 遍历数组遍历数据的函数
int search_index(int *arr,int len,int target){
  for (int i = 0; i < len;i++)
    if(arr[i]==target)
      return i;
  return -1;
}
int main()
{
  // 需求:从数组中查找某个目标数据,找到则返回索引,找不到则返回-1
  // int nums[] = {10, 20, 30, 40, 50, 60, 70, 80};
  // int target = 30;                      // 目标数据
  // int len = sizeof(nums) / sizeof(int); // 长度
  // int index = binary_search(nums, len, target);
  // printf("索引:%d\n", index);
  printf("\n======================\n");
  int nums[] = {10, 20, 30, 40, 50, 60, 70, 80};
  int target = 30;                      // 目标数据
  int len = sizeof(nums) / sizeof(int); // 长度
  int index = search_index(nums, len, target);
  printf("索引:%d\n", index);

  return 0;
}