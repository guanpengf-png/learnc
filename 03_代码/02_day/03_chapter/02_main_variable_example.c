#include <stdio.h>

int main()
{
  // 变量的三个要素:数据类型名 变量名 数据值
  // = 赋值符号,把符号右侧的数据值存储到左侧的变量中
  int num = 10; // 声明了一个int类型的变量,起名为num,并赋值为10
  printf("num的值是:%d\n", num);
  num = 20; // 为变量重新赋值
  printf("num的值是:%d\n", num);
  num = 100; // 为变量重新赋值
  printf("num的值是:%d\n", num);
  return 0;
}