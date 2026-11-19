#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
int main()
{
  int *ptr = (int *)malloc(sizeof(int) * 200);
  printf("ptr指针指向的内存地址:%p\n", ptr);
  printf("ptr指针指向的内存空间所占的字节数:%zu\n", _msize(ptr));
  ptr = realloc(ptr, sizeof(int) * 10000);
  printf("ptr指针指向的内存地址:%p\n", ptr);
  printf("ptr指针指向的内存空间所占的字节数:%zu\n", _msize(ptr));
   ptr = realloc(ptr, sizeof(int) * 2);
  printf("ptr指针指向的内存地址:%p\n", ptr);
  printf("ptr指针指向的内存空间所占的字节数:%zu\n", _msize(ptr));

  return 0;
}