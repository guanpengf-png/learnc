/*
 * 章节：递归求阶乘
 * 知识点：
 *   n! = n * (n - 1)!，并且 1! = 1。0! 也是 1，如果要算 0 就再加一个条件。
 *   int 放不下太大的阶乘。8! = 40320，还在 int 里面；13! 就会溢出 32 位 int。
 */
#include <stdio.h>

static int factorial(int n)
{
    if (n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main(void)
{
    printf("8! = %d\n", factorial(8));
    return 0;
}
