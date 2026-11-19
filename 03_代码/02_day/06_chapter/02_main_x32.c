#include <stdio.h>
#include <stdint.h>
int main()
{
  int32_t x32 = 100;
  // x32变量到底占几个字节
  printf("x32变量存储的数据所占的字节数是:%zu\n",sizeof(x32)); // 4 个字节
  printf("x32变量的值是:%d\n",x32);

  return 0;
}