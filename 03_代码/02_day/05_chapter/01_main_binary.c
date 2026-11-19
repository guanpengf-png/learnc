#include <stdio.h>

int main(){
  int num1 = 0b1101; // 二进制
  int num2 = 101; // 十进制
  int num3 = 0x3A; // 十六进制
  printf("num1对应十进制的结果:%d\n",num1); // 13
  printf("num2对应十进制的结果:%d\n",num2); // 101
  printf("num3对应十进制的结果:%d\n",num3); // 58
  // 不同的进制数据可以直接进行运算
  printf("结果:%d\n", 0b1101 + 101 + 0x3A);

  int num4 = 27; // 十进制
  // %x格式占位符输出的数据值是没有前缀  0x
  printf("num4变量中的数据以十六进制的方式进行输出:%x\n",num4);
  printf("num4变量中的数据以十六进制的方式进行输出:%X\n",num4);
  printf("num4变量中的数据以十六进制的方式进行输出:%#x\n",num4);
  printf("num4变量中的数据以十六进制的方式进行输出:%#X\n",num4);
  return 0;
}