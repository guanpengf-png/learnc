/*
 * 章节：作用域练习
 * 知识点：
 *   先看使用这个名字的位置，再决定它指的是哪一层。
 *   内层有同名变量，就用内层；内层没有，就向外找到全局。
 *   讲义第一题把四次输出注释成 205、210、200、250，和代码不符。
 *   正确过程：price 一开始是 200；test01 只打印，不修改；test2 把它改成 250。
 */
#include <stdio.h>

double price = 200.0;

void test01(void)
{
    printf("test01: %.2f\n", price);
}

void test2(void)
{
    price = 250.0;
    printf("test2: %.2f\n", price);
}

int a = 10;

void func1(void)
{
    int a = 20;
    printf("func1 a = %d\n", a);
}

void func2(int a)
{
    printf("func2 a = %d\n", a);
}

void func3(void)
{
    printf("func3 a = %d\n", a);
}

int main(void)
{
    printf("main 里的 price: %.2f\n", price);
    test01();
    test2();
    test01();

    int a = 30;
    func1();
    func2(a);
    func3();
    {
        int a = 40;
        printf("block a = %d\n", a);
    }
    printf("main a = %d\n", a);
    return 0;
}
