/*
 * 章节：函数的定义和调用
 * 知识点：
 *   函数是一段起了名字的代码，用来完成一件事，可以反复调用。
 *   写法：返回类型 函数名(参数列表) { 函数体; return 返回值; }
 *   没有返回值就写 void，调用时也不要去接结果。
 *   调用就是函数名后面加上圆括号。每调用一次，函数体执行一遍。
 *   函数之间互相独立，不能在一个函数里面再定义另一个函数。
 *   程序从 main 开始。main 可以调用别的函数，别的函数不要反过来调用 main，main 也不要调用自己。
 */
#include <stdio.h>

void show_hello(void)
{
    printf("啊捏哈塞呦!\n");
}

int get_max_num(int x, int y)
{
    return x > y ? x : y;
}

int add(int x, int y)
{
    return x + y;
}

int main(void)
{
    show_hello();
    printf("最大值: %d\n", get_max_num(10, 5));
    printf("和: %d\n", add(10, 5));
    return 0;
}
