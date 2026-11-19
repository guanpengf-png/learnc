#include <stdio.h>
#include <stdlib.h>
int main()
{
  // 需求:存储10个整数,手动分配内存的方式实现
  int len = 10; // 长度为10
  int *ptr = (int *)malloc(sizeof(int) * len);
  // for (int i = 0; i < len; i++)
  //   printf("数据值:%d,内存地址:%p\n", *(ptr + i), &ptr[i]);
  // printf("\n================\n");
  // 判断内存分配是否成功
  if (ptr == NULL)
  {
    printf("内存分配失败\n");
    return 0;
  }
  // 遍历的方式移动指针,设置对应位置的内存存储数据
  for (int i = 0; i < len; i++)
  {
    // *(ptr + i) = i * 10;
    ptr[i] = i * 10;
  }
  for (int i = 0; i < len; i++)
    printf("数据值:%d,内存地址:%p\n", *(ptr + i), &ptr[i]);
  // printf("数据值:%d,内存地址:%p\n", ptr[i], ptr + i);
  free(ptr);  // 释放内存
  ptr = NULL; // 重置为空指针


  // malloc函数分配的内存空间,里面是存在垃圾值的
  return 0;
}