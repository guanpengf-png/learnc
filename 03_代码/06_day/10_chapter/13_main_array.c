#include <stdio.h>
int main()
{
  int nums1[] = {10, 20, 30};
  for (int i = 0; i < 3; i++)
    printf("索引:%d,元素值:%d,对应的内存地址:%p\n", i, nums1[i], &nums1[i]);
  printf("\n=============\n");
  int nums2[2][3] = {100, 200, 300, 400, 500, 600};
  int rows = sizeof(nums2) / sizeof(nums2[0]); // 计算行
  int cols = sizeof(nums2[0]) / sizeof(int);   // 计算列
  for (int i = 0; i < rows; i++)               // 控制的是行
    for (int j = 0; j < cols; j++)             // 控制的是列
      printf("索引:%d,元素值:%d,对应的内存地址:%p\n", i, nums2[i][j], &nums2[i][j]);

  // 二维数组本质和一维数组在内存中存储方式是一样的,也是连续的,也是属于线性的

  return 0;
}