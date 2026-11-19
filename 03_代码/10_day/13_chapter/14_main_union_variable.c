#include <stdio.h>
// 共用体类型变量声明的三种方式:
// 1  先定义共用体类型,后声明变量
// 2  定义共用体类型的同时直接声明变量
// 3  定义共用体类型的同时直接声明变量,省略类型名
// 共用体类型的定义位置:函数外部/函数内部


// 1  先定义共用体类型,后声明变量
union Data1{
  int num;
  char ch;
};
// 声明变量的语法: union Data1 变量名;
union Data1 dt1;
// 2  定义共用体类型的同时直接声明变量
union DATA2{
  char ch;
  float f;
  int n;
}dt2,dt3;
// 3  定义共用体类型的同时直接声明变量,省略类型名
union{
  char ch;
  double d;
  int n;
}dt4,dt5,dt6;
int main()
{
  
  
  return 0;
}