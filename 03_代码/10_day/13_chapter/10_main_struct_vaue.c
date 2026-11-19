#include <stdio.h>
struct Student{
  int id; // 学号
  char *name; // 姓名
};
// 先定义结构体类型,后声明变量并赋值
struct Student stu1 = {101, "小甜甜"};
// 定义结构体类型的同时直接声明变量并赋值
struct Teacher{
  char *name; // 姓名
  int age; // 年龄
} t1={"大甜甜",20},t2={.age=30,.name="中甜甜"};

// 定义结构体类型的同时直接声明变量并赋值
struct {
  int age; // 年龄
  char gender; // 性别
} t3={20,'f'},t4={.gender='m',.age=20};

// 枚举类型
// enum Gender
// {
//   female,
//   male
// };
int main()
{
  
  
  return 0;
}