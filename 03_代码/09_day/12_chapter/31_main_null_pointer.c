#include <stdio.h>
int main()
{
  // 空指针:指针指向了NULL
  // 当指针指向的是NULL,就是空指针,是不能解引用的,目的是为了释放该内存空间
  // int num = 10;
  // int *ptr = &num;
  // printf("ptr指针指向的内存地址:%p\n", ptr);
  // printf("ptr指针指向的内存地址对应空间中数据:%d\n", *ptr);
  // ptr = NULL;
  // printf("ptr指针指向的内存地址:%p\n", ptr);
  // printf("ptr指针指向的内存地址对应空间中数据:%d\n", *ptr);

  int *ptr = NULL; // 初始化指针为空指针
  int num = 10;
  ptr = &num;

  return 0;
}