/*
 * 章节：算术运算符
 * 知识点：
 *   一元 + - 只改变符号，不修改原来的变量。
 *   两个整数相除，小数部分直接丢掉，不是四舍五入。
 *   想得到小数，至少要有一个操作数是浮点数。
 *   double d = x / y 仍然会丢精度：除法先发生，得到的已经是整数。
 *   % 是取余。结果的符号跟第一个操作数相同。
 */
#include <stdio.h>

int main(void)
{
    int x1 = 10;
    printf("x1 = %d, -x1 = %d, +x1 = %d\n", x1, -x1, +x1);

    int x2 = -10;
    printf("x2 = %d, -x2 = %d, +x2 = %d\n", x2, -x2, +x2);

    int x = 10;
    int y = 3;
    printf("x + y = %d\n", x + y);
    printf("x - y = %d\n", x - y);
    printf("x * y = %d\n", x * y);
    printf("x / y = %d\n", x / y);
    printf("x / y = %f\n", x * 1.0 / y);

    double truncated = x / y;
    printf("先整除再转 double = %f\n", truncated);
    printf("先转 double 再相除 = %f\n", (double)x / y);

    printf("10 %% 3 = %d\n", 10 % 3);
    printf("-10 %% 3 = %d\n", -10 % 3);
    printf("-10 %% -3 = %d\n", -10 % -3);
    printf("10 %% -3 = %d\n", 10 % -3);
    return 0;
}
