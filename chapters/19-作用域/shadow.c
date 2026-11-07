/*
 * 章节：同名遮蔽
 * 知识点：
 *   函数里如果定义了和全局变量同名的局部变量，函数里用的是局部那个。
 *   没有同名局部变量时，用的才是全局变量。
 *   讲义的输出写成“内部 a = 300，外部 a = 120、b = 200”，和代码不符。
 *   a += 10 改的是局部 a，全局 a 仍然是 100。
 *   b += 20 没有局部 b，改的是全局 b，所以函数返回后 b 是 220。
 */
#include <stdio.h>

int a = 100;
int b = 200;

void show_ab(void)
{
    int a = 300;
    a += 10;
    b += 20;
    printf("函数内部 a = %d, b = %d\n", a, b);
}

int main(void)
{
    show_ab();
    printf("函数外部 a = %d, b = %d\n", a, b);
    return 0;
}
