/*
 * 章节：关系运算符和逻辑运算符
 * 知识点：
 *   关系运算的结果是整数：成立为 1，不成立为 0。
 *   == 才是比较相等。= 是赋值，写错会变成修改变量。
 *   && 两边都真才真。左边为假时右边不计算，这叫短路。
 *   || 有一边真就真。左边为真时右边不计算。
 *   ! 取反：!0 是 1，!非0 是 0。
 */
#include <stdio.h>

int main(void)
{
    int a = 10;
    int b = 5;
    printf("a == b : %d\n", a == b);
    printf("a != b : %d\n", a != b);
    printf("a > b  : %d\n", a > b);
    printf("a < b  : %d\n", a < b);
    printf("a >= b : %d\n", a >= b);
    printf("a <= b : %d\n", a <= b);

    a = 10;
    b = 20;
    printf("a > 5 && b > 10 : %d\n", a > 5 && b > 10);
    printf("短路与之前 b = %d\n", b);
    printf("a < 5 && b++ > 10 : %d\n", a < 5 && b++ > 10);
    printf("短路与之后 b 仍然是 %d，b++ 没执行\n", b);

    printf("a > 10 || b < 50 : %d\n", a > 10 || b < 50);
    printf("短路或之前 b = %d\n", b);
    printf("a > 5 || b++ > 10 : %d\n", a > 5 || b++ > 10);
    printf("短路或之后 b 仍然是 %d，b++ 没执行\n", b);

    printf("!0 = %d\n", !0);
    return 0;
}
