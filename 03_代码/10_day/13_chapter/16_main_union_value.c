#include <stdio.h>

int main()
{
  // 先定义共用体类型,然后声明变量,后赋值
  union Data1
  {
    char ch;
    int num;
  };
  // 声明变量,并赋值
  union Data1 dt;
  dt.ch = 'A';

  // 定义共用体类型的同时直接声明变量并赋值
  union Data2
  {
    short s;
    char ch;
  } dt2 = {10}, dt3 = {.ch = 'B'};

    // 定义共用体类型的同时直接声明变量并赋值
  union Data3
  {
    short s;
    char ch;
    int n;
  } dt4={.ch='A'},dt5={.n=120};

  return 0;
}