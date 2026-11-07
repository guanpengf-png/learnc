/*
 * 章节：全局作用域
 * 知识点：
 *   写在所有函数外面的变量、常量和数组，整个程序都能用，叫全局变量。
 *   全局变量如果没有写初值，会自动变成 0。全局字符数组的每个元素自动是 '\0'。
 *   局部变量没有这个待遇。不初始化的局部变量是垃圾值，读它是未定义行为，不要打印。
 *   同一个作用域里不能有两个同名标识符。
 */
#include <stdio.h>

int num = 100;
const double PI = 3.1415926;
char msgs[] = "hello";

int a;
double b;
char ch;
int nums[5];
char chs[5];

void show_func(void)
{
    printf("函数内部 num = %d, PI = %.2f, msgs = %s\n", num, PI, msgs);
    num += 10;
}

int main(void)
{
    show_func();
    printf("main 里 num = %d, PI = %.2f, msgs = %s\n", num, PI, msgs);

    printf("未初始化的全局变量: a = %d, b = %.2f, ch 的编码 = %d\n", a, b, ch);
    printf("未初始化的全局数组:");
    for (int i = 0; i < (int)(sizeof nums / sizeof nums[0]); i++) {
        printf(" %d", nums[i]);
    }
    printf("\n未初始化的全局字符数组编码:");
    for (int i = 0; i < (int)(sizeof chs / sizeof chs[0]); i++) {
        printf(" %d", chs[i]);
    }
    printf("\n");
    return 0;
}
