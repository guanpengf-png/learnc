#include <stdio.h>
// typedef关键字:用来起别名的(为类型起别名)
int main()
{
  // 1. 为基本类型起别名
  // typedef int integer;
  // int num1 = 10;
  // integer num2 = 20;
  // // 2.为类型起多个别名
  // typedef int apple, banana, cantaloupe;
  // apple x = 10;
  // banana y = 20;
  // cantaloupe z = 30;
  // typedef unsigned long long size_x;
  // size_x ll1 = 10LL;
  // // 3. 为结构体类型起别名
  // struct Per
  // {
  //   int id;
  //   char *name;
  // };
  // // 方式1:
  // typedef struct Per Person;
  // struct Per p1;
  // Person p2; // 结构体类型的变量
  // // 方式2:
  // typedef struct
  // {
  //   int id;
  //   char *name;
  //   int age;
  // } Student;
  // Student stu = {101, "小甜甜", 20};
  // 4. 为共用体类型起别名
  // typedef union
  // {
  // } Data;
  // Data dt = {};

  // 5. 为指针起别名
  // int num = 10;
  // typedef int *Pointer;
  // Pointer ptr = &num; // ptr指针变量的类型是Pointer

  // typedef char *string;
  // string str = "hello";
  // int num = 10;

  // typedef int bool;
  // bool flag = 1;

  // 6. 为数组起别名
  // int nums[] = {10,20,30};
  // typedef int Array[3];
  // // 声明一个数组,arr的类型是Array
  // Array arr = {10,20,30};

  // 7. 为数组指针起别名
  // int nums[] = {10, 20, 30};
  // // int(*ptr)[3] = &nums;
  // typedef int(*Pointer_Array)[3];
  // Pointer_Array pa = &nums;

  return 0;
}