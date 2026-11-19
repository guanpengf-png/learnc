#include <stdio.h>
#include <stdlib.h>
int main()
{
  int len = 1;
  // 向系统申请(4个字节)内存存储空间,并返回对应的内存地址,存储到指针变量中
  int *ptr = (int *)malloc(sizeof(int) * len);
  // 判断内存分配是否成功
  if (ptr == NULL)
  {
    printf("内存分配失败\n");
    return 0;
  }
  // 存储数据
  *ptr = 100;
  printf("内存地址:%p,对应数据:%d\n",ptr,*ptr);
  free(ptr); // 释放内存
  ptr = NULL; // 重置为空指针

  return 0;
}