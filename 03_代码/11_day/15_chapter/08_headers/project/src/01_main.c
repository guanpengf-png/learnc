
#include <stdio.h>
// #include <标准头文件/系统的头文件/标准的库文件>
// #include "自定义头文件"
// #include "自定义头文件" // 不要加分号;
// #include "绝对路径/自定义头文件"
// #include "相对路径/自定义头文件"

// 使用绝对路径的方式引入文件
// #include "D:\2026_11_02\code\15_chapter\08_headers\project\src\02_my_header.h"
// 使用相对路径:    ./-----当前目录
// ../------上一级目录
#include "./02_my_header.h"
// #include "02_my_header.h" // 可省略./
#include "./includes/03_my_header.h"
#include "./../04_my_header.h"
#include "./../incs/05_my_header.h"
#include "./../../06_my_header.h"
int main()
{
  func2();
  func3();
  func4();
  func5();
  func6();

  return 0;
}