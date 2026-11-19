#include <stdio.h>
// 枚举类型变量的赋值
// 定义了一个性别的枚举类型
// enum gender
// {
//   female,
//   male
// };

// int main()
// {
//   // 声明一个枚举类型变量
//   // 直接使用枚举元素值赋值
//   enum gender g1;
//   g1 = female; // 直接赋值
//   enum gender g2 = male;
//   // 赋值方式2:可以使用大括号+枚举元素值的方式赋值
//   enum gender g3 = {female};

//   return 0;
// }


#include <stdio.h>
// 定义枚举类型,再声明变量赋值
enum gender
{
  female,
  male
};
enum gender g = female; // 先定义枚举类型,声明变量后赋值(直接使用枚举元素值赋值)

enum SEASON
{
  spring,
  summer,
  autumn,
  winter
} s1={spring},s2={autumn}; // 定义枚举类型的同时直接声明变量并赋值,

enum 
{
  spring,
  summer,
  autumn,
  winter
} s3={spring},s4={autumn}; // 定义枚举类型的同时省略枚举类型名,直接声明变量并赋值,
int main()
{
  
  
  return 0;
}