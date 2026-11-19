#include <stdio.h>
void f1()
{
  printf("f1函数\n");
  // return 10;
}

int f2()
{
  printf("f2函数\n");
  return;
}

int f3()
{
  return 1000;
}

void f4(int x)
{
  if (x > 10)
  {
    printf("%d\n", 100);
    return;
  }
  printf("%d\n", 1000);
}
int main()
{

  // void result1= f1(); // 函数调用
  // int result2 = f2(); // 调用函数
  // printf("结果:%d\n",result2);
  // int result3 = f3();
  // printf("结果:%d\n",result3);
  f4(50); // 函数调用
  // 函数的返回值类型及返回值的理解
  // 函数的返回值分为三种情况:
  // 1. 函数无返回值
  // 2. 函数有不明确的返回值(返回的是不明确的数据值)
  // 3. 函数有返回值

  /*
     1. 函数无返回值:指的是函数的返回值类型是void,函数内部有无return,或return后面有无数据
     void表示空类型---无效类型

     2. 函数有不明确的返回值:指的是函数的返回值类型不是void,且函数内部没有return,或者return后面没有明确的数据值

     3. 函数有返回值:指的是函数的返回值类型不是void,且函数内部return后面有明确的数据值


  */

  return 0;
}