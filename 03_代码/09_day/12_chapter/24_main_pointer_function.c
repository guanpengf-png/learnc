#include <stdio.h>
// 通过指针函数实现,两个数字中的最大值,返回最大值的内存地址
int *get_max_pointer(int a, int b)
{
  static int max_num; // 静态局部变量只在函数第一次调用的时候进行初始化一次
  max_num = a > b ? a : b;
  return &max_num;
}
int main()
{

  int x = 10, y = 20;
  int *ptr = get_max_pointer(x, y);
  printf("ptr指针指向的内存地址:%p\n", ptr);
  printf("最大值是:%d\n", *ptr);

  // 指针函数的声明或定义语法:
  /*
    返回值类型 * 函数名([参数列表]){
      函数体语句代码;
    }
    指针函数的类型:返回值类型 * ([参数列表])

    指针函数不能返回局部变量的内存地址,可以返回静态局部变量的内存地址

  */

  return 0;
}