#include <stdio.h>
// 定义学生的结构体类型
struct Student{
  int id;      // 学号
  char *name;  // 姓名
  char gender; // 性别
  int age;     // 年龄
};
int main()
{

  // 1. 先声明结构体类型变量,后赋值
  struct Student stu;
  // 结构体类型变量赋值的语法: 结构体变量名.成员名 = 值;
  // 结构体类型变量中成员获取值的语法: 结构体变量.成员名
  stu.id = 101;
  stu.name = "小甜甜";
  stu.gender = 'f';
  stu.age = 20;
  printf("学号:%d,姓名:%s,性别:%c,年龄:%d\n", stu.id, stu.name, stu.gender, stu.age);

  // 2. 声明结构体类型的同时直接声明变量并赋值，下面的赋值方式是数据值和成员声明的时候的顺序要一致
  // 赋值的时候使用的是大括号和值的形式
  struct Student stu2 = {102, "大甜甜", 'm', 30};
  printf("学号:%d,姓名:%s,性别:%c,年龄:%d\n", stu2.id, stu2.name, stu2.gender, stu2.age);
  //  struct Student stu3 = {"中甜甜",20,'f',103}; // 不要这么做
  // 结构体变量赋值,使用大括号和.语法同时赋值,顺序可以不一样
  struct Student stu3 = {.name = "中甜甜", .id = 103, .gender = 'm', .age = 40};
  printf("学号:%d,姓名:%s,性别:%c,年龄:%d\n", stu3.id, stu3.name, stu3.gender, stu3.age);

  return 0;
}