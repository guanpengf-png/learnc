#include <stdio.h>
int main()
{
  // f---女 m---男
  // char chs[100]; // 字符数组
  // char name[] = "小甜甜"; // 名字
  // int age = 20; // 年龄
  // char gender = 'f'; // 性别
  // // 把指定格式的字符串数据值存储到字符数组中
  // sprintf(chs, "年龄是%d,性别是%c,名字是%s", age, gender, name);
  // printf("chs=%s\n",chs);

  char chs[] = "年龄是20岁,性别是:f,姓名是:小甜甜";
  int age; // 年龄
  char name[20]; // 名字
  char gender; // 性别
  // 把字符数组中的数据按照指定的格式提取出来
  sscanf(chs,"年龄是%d岁,性别是:%c,姓名是:%s",&age,&gender,name);
  printf("名字:%s,年龄:%d,性别:%c\n",name,age,gender);

  return 0;
}