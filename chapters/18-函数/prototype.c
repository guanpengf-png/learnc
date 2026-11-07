/*
 * 章节：函数原型
 * 知识点：
 *   编译器必须在调用之前看见函数的返回类型和参数类型。
 *   函数体可以写在 main 后面，但 main 前面要先放一行原型。
 *   原型不需要函数体，参数名也可以省略，只留类型：int get_sum(int, int);
 *   如果函数在调用之前已经完整定义过，原型就可以不写。
 */
#include <stdio.h>

int get_sum(int, int);

int main(void)
{
    int result = get_sum(10, 20);
    printf("%d\n", result);
    return 0;
}

int get_sum(int x, int y)
{
    return x + y;
}
