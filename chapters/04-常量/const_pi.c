/*
 * 章节：用 const 定义常量
 * 知识点：
 *   const 声明一个只读变量，程序运行时不能再给它赋值。
 *   它有类型，编译器会做类型检查；作用域限于定义它的那一块。
 *   #define 没有类型、没有作用域，只是替换文本。
 *   日常优先用 const。需要在预处理阶段就替换、或定义编译开关时再用 #define。
 *
 * 试运行输入：2.0
 */
#include <stdio.h>

const double PI = 3.1415;

int main(void)
{
    double r;
    printf("请您输入圆的半径 \n");
    if (scanf("%lf", &r) != 1) {
        return 1;
    }

    printf("圆的面积为%.2f\n", PI * r * r);
    return 0;
}
