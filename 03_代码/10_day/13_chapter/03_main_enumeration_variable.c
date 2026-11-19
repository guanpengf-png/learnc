#include <stdio.h>
// 枚举类型的变量声明有三种方式:
// 1.先定义枚举类型,后声明变量
// 2.定义枚举类型的同时声明变量
// 3.定义枚举类型的同时声明变量,省略枚举类型的名字

// 定义一个性别的枚举类型
enum GENDER
{
  female,
  male
};
// 1.先定义枚举类型,后声明变量的语法: enum 枚举类型名 变量名;
enum GENDER g;
// 2.定义枚举类型的同时声明变量
enum SEASON
{
  spring,
  summer,
  autumn,
  winter
} s1,s2, s3;
// 3.定义枚举类型的同时声明变量,省略枚举类型的名字
enum
{
  spring,
  summer,
  autumn,
  winter
} s4,s5, s6;
int main()
{

  return 0;
}