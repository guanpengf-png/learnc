#include <stdio.h>
// 结构体指针:就是指针,指向的是结构体变量的内存地址
struct Student
{

  // char name[20]; // 姓名
  char *name;  // 姓名
  char gender; // 性别
  int age;     // 年龄
};
int main()
{
  // 声明结构体类型的变量并赋值
  struct Student stu = {"小甜甜", 'f', 20};
  // 使用结构体变量.成员读取值
  printf("姓名:%s,年龄:%d,性别:%c\n", stu.name, stu.age, stu.gender);
  // 声明结构体类型的指针并指向结构体变量的内存地址
  struct Student *ptr = &stu;
  // 使用结构体指针->成员名 读取值
  printf("姓名:%s,性别:%c,年龄:%d\n", ptr->name, ptr->gender, ptr->age);
  // 使用结构体指针->成员 = 值 赋值
  ptr->name = "大甜甜";
  ptr->age = 50;
  ptr->gender = 'm';
  // 解引用结构体指针读取成员值
  printf("姓名:%s,性别:%c,年龄:%d\n", (*ptr).name, (*ptr).gender, (*ptr).age);
  printf("姓名:%s,年龄:%d,性别:%c\n", stu.name, stu.age, stu.gender);

  // 声明结构体类型的变量
  // struct Student stu = {"小甜甜",'f',20};
  // printf("姓名:%s,年龄:%d,性别:%c\n",stu.name,stu.age,stu.gender);
  // printf("结构体变量的内存地址:%p\n",&stu);
  // printf("结构体变量的内存地址:%p\n",stu);
  // printf("结构体变量的内存地址:%p\n",stu.name);
  // 声明结构体类型的指针
  // struct Student *ptr = &stu;
  // 通过结构指针为结构体成员赋值和取值语法:
  // ->   成员访问符
  // 结构体指针名->成员名 = 值; 赋值语法
  // 结构体指针名->成员名; // 获取值的语法
  // (*结构体指针).成员名 = 值; 赋值语法
  // (*结构体指针).成员名; 取值语法
  // printf("姓名:%s,性别:%c,年龄:%d\n",ptr->name,ptr->gender,ptr->age);
  // ptr->name = "大甜甜";
  // ptr->age = 50;
  // ptr->gender = 'm';
  // printf("姓名:%s,性别:%c,年龄:%d\n",(*ptr).name,(*ptr).gender,(*ptr).age);
  // printf("姓名:%s,年龄:%d,性别:%c\n",stu.name,stu.age,stu.gender);

  // 1. 先声明结构体指针,后赋值
  // 语法: struct 结构体类型名 * 指针名; 指针名 = 结构体变量的内存地址;
  // struct Student *ptr1;
  // ptr1 = &stu;
  // 2. 声明结构体指针的同时赋值
  // struct Student *ptr2 = &stu;

  return 0;
}