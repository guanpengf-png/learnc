/*
 * 章节：形参和实参
 * 知识点：
 *   形参：定义函数时，圆括号里声明的变量。它是这个函数的局部变量。
 *   实参：调用时真正传进去的常量、变量或表达式。
 *   实参的个数和类型要和形参一致。
 *   传的是值的副本。函数里修改形参，不会改掉调用处那个变量。
 *   如果希望函数改掉调用者的数据，后面要传指针，见指针一章。
 */
#include <stdio.h>

int get_max(int a, int b)
{
    return a > b ? a : b;
}

int main(void)
{
    int x = 10;
    int y = 20;
    int max_num = get_max(x, y);
    printf("最大值: %d\n", max_num);
    printf("调用之后 x 仍然是 %d, y 仍然是 %d\n", x, y);
    return 0;
}
