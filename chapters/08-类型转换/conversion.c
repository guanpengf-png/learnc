/*
 * 章节：数据类型转换
 * 知识点：
 *   隐式转换由编译器自动做，方向是窄类型转向宽类型，避免丢失精度。
 *     整数之间：窄整数升成宽整数。
 *     浮点之间：精度低的升成精度高的。
 *     整数和浮点：整数先变成浮点。
 *   有符号转无符号不是取绝对值，而是按补码位模式重新解释。
 *     32 位下 -10 变成 4294967286，再加 5 得到 4294967291。
 *   赋值时，右边会转成左边的类型。宽类型赋给窄类型会丢精度。
 *   明确要转换时写成强制转换：(类型名)表达式。
 *     (int)d1 + (int)d2  先各自截断再相加
 *     (int)(d1 + d2)     先相加再截断
 */
#include <stdio.h>

int main(void)
{
    short s1 = 10;
    int i1 = 20;
    printf("s1 + i1 = %d\n", s1 + i1); /* short 先提升成 int */

    signed int negative = -10;
    unsigned int positive = 5u;
    unsigned int wrapped = (unsigned int)negative + positive;
    printf("a + b = %u\n", wrapped);

    float f1 = 35.45f;
    double d1 = 50.123;
    printf("f1 + d1 = %.10f\n", (double)f1 + d1);

    int i2 = 100;
    double d2 = 10.12345;
    printf("i2 + d2 = %.2f\n", i2 + d2);

    int i3 = 100;
    double d3 = i3; /* 窄类型赋给宽类型，值不变 */
    printf("d3 = %.2f\n", d3);

    double d4 = 100.12345;
    int i4 = (int)d4; /* 小数部分被丢掉，结果是 100 */
    printf("i4 = %d\n", i4);

    double left = 3.987;
    double right = 4.235;
    int sum1 = (int)(left + right);          /* 8.222 -> 8 */
    int sum2 = (int)left + (int)right;       /* 3 + 4 = 7 */
    int sum3 = (int)(left + right);
    int sum4 = (int)(2.5 * 10 + 2 * 1.5);    /* 25 + 3 = 28 */
    printf("sum1 = %d\n", sum1);
    printf("sum2 = %d\n", sum2);
    printf("sum3 = %d\n", sum3);
    printf("sum4 = %d\n", sum4);
    return 0;
}
