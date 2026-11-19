#include <stdio.h>
#include <stdlib.h>
int main()
{
  // 申请内存空间,用来存储一个整数
  void * ptr =  malloc(4);
  // 判断内存分配是否成功
  if(ptr==NULL){
    printf("内存分配失败\n");
    return 0;
  }
  printf("ptr指针指向的地址:%p\n",ptr);
  // printf("ptr指针指向的内存空间中的数据:%d\n",*ptr);
  int *ptr2 = (int *)ptr; // 指针变量进行类型转换
  *ptr2 = 100; // 存储数据
  printf("内存地址:%p,数据值:%d\n",ptr2,*ptr2);
  // 释放内存空间(不能不释放/也没有必要释放多次)
  free(ptr2);
  ptr2 = NULL; // 重置为空指针
  // printf("内存地址:%p\n", ptr2);
  // printf("数据值是:%d\n",*ptr2);

  return 0;
}