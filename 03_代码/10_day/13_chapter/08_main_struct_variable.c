#include <stdio.h>
/*
  结构体类型变量声明三种方式:
  1. 先定义结构体类型,后声明变量

  2. 定义结构体类型的同时直接声明变量

  3. 定义结构体类型的同时直接声明变量,省略类型名



*/

  // 1. 先定义结构体类型,后声明变量
struct Student{
  int id; // 学号
  char name[20]; // 姓名
  int age; // 年龄
};
// 声明变量语法: struct 结构体类型名 变量名;
struct Student stu;
// 2. 定义结构体类型的同时直接声明变量
struct Teacher{
  char name[20]; // 姓名
  int age; // 年龄
  double salary; // 工资
}t1,t2,t3;

// 3. 定义结构体类型的同时直接声明变量,省略类型名
struct{
  char name[20]; // 姓名
  int age; // 年龄
  char color[20]; // 颜色
}d1,d2,d3;
int main()
{
 
  
  return 0;
}