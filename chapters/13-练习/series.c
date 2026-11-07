/*
 * 练习：1 - 1/2 + 1/3 - 1/4 + ... + 1/100
 * 知识点：
 *   1 / i 在两个整数之间做除法，i > 1 时结果是 0，和会一直是 1。
 *   必须写成 1.0 / i，让除法变成浮点除法。
 *   和要用 double 保存。
 *   符号每一项翻转一次：用一个 sign，加完就写成 -sign。
 */
#include <stdio.h>

int main(void)
{
    double sum = 0.0;
    int sign = 1;
    for (int i = 1; i <= 100; i++) {
        sum += sign * (1.0 / i);
        sign = -sign;
    }
    printf("sum = %.6f\n", sum);
    return 0;
}
