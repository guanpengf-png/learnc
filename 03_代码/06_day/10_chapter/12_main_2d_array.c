#include <stdio.h>
int main()
{
  // 二维数组:三行四列
  // int nums[3][4] = {
  //     {10, 20, 30, 40},
  //     {50, 60, 70, 80},
  //     {90, 91, 92, 93},
  // };
  // // 计算总字节数
  // printf("二维数组总字节数:%zu\n",sizeof(nums)); // 48个字节
  // printf("第一个数组的元素的总字节数:%zu\n",sizeof(nums[0])); // 16
  // // 总行数= 总字节数/单个数组元素的总字节数
  // int rows = sizeof(nums) / sizeof(nums[0]); // 3
  // printf("行数:%d\n",rows);
  // // 总列数 = 单个数组元素的总字节数/单个数据的字节数
  // int cols = sizeof(nums[0]) / sizeof(nums[0][0]); // 4
  // printf("列数:%d\n",cols);
  // // 总列数 = 单个数组元素的总字节数/具体某个数据的类型所占的字节数
  // int cols2 = sizeof(nums[0]) / sizeof(int);
  // printf("列数:%d\n",cols2);

  // int rows; // 行
  // int cols; // 列
  // for (int i = 0; i < rows; i++) // 控制的是行
  // {
  //   for (int j = 0; j < cols; j++) // 控制的是列
  //     printf("%d ",nums[i][j]);
  //   printf("\n");
  // }

  // 三行四列的二维数组,nums二维数组的类型是int[3][4]
  int nums[3][4] = {
      {10, 20, 30, 40},
      {50, 60, 70, 80},
      {90, 91, 92, 93},
  };

  int rows = sizeof(nums) / sizeof(nums[0]); // 计算行
  //  int cols = sizeof(nums[0]) / sizeof(nums[0][0]); // 计算列
  int cols = sizeof(nums[0]) / sizeof(int); // 计算列
  for (int i = 0; i < rows; i++)            // 控制的是行
  {
    for (int j = 0; j < cols; j++) // 控制的是列
      printf("%d ", nums[i][j]);   // 具体的数据值
    printf("\n");                  // 换行
  }

  return 0;
}