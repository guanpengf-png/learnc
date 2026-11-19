#include <stdio.h>
#include <stdlib.h>
int main()
{
  int len = 5;
  int *ptr = (int *)calloc(len, sizeof(int)); // 分配20个字节的内存
  if (ptr == NULL)
  {
    printf("内存分配失败\n");
    return 0;
  }
  // calloc函数分配内存,空间中默认是0
  for (int i = 0; i < len; i++)
    printf("%d====%p\n", ptr[i], ptr + i);
  printf("\n=================\n");
  // 遍历赋值
  for (int i = 0; i < len; i++)
    ptr[i] = i * 10;
  for (int i = 0; i < len; i++)
    printf("%d====%p\n", ptr[i], ptr + i);
  // 释放内存
  free(ptr);
  ptr = NULL;
  return 0;
}