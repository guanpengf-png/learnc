/*
 * 章节：多文件里的全局变量
 * 知识点：
 *   这个文件提供变量和函数，真正的 main 在 file02.c。
 *   两个文件要一起编译：
 *     cc -std=c11 -Wall -Wextra -Wpedantic file01.c file02.c -o extern_demo
 *   普通全局变量、全局数组、全局函数，别的文件可以用 extern 声明后使用。
 *   static 全局变量和 static 函数只在当前文件可见。别的文件即使写了 extern，链接也会失败。
 *   定义是 const double 时，extern 也必须写成 const double，类型要一致。
 */
#include <stdio.h>

int a = 100;
const double PI = 3.1415;
char chs[] = "hello every";

static int a1 = 200;
static char chs1[] = "only this file";

void func(void)
{
    printf("function func\n");
    printf("本文件才能看见 a1 = %d, chs1 = %s\n", a1, chs1);
}

static void func1(void)
{
    printf("static 函数 func1，别的文件不能调用\n");
}

void call_static(void)
{
    func1();
}
