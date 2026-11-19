#include <stdio.h>
// 使用typedef起别名的方式,实现需求
typedef char *string; // 为字符指针类型起别名
typedef struct {
  int id; // 编号
  string name; // 姓名
  int age; // 年龄
}Person;
int main()
{
  Person p; // 结构体类型的变量
  p.name = "小甜甜";
  p.age = 20;
  p.id = 101;
  printf("编号:%d,名字:%s,年龄:%d\n",p.id,p.name,p.age);

  return 0;
}