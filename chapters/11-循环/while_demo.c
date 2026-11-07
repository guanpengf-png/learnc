/*
 * 章节：while 循环
 * 知识点：
 *   while (条件) { 循环体 } 先判断，后执行。条件一开始就是假，循环体一次都不跑。
 *   计数循环要有三件事：循环变量初始化、用它写条件、每次让它靠近结束。
 *   漏掉 i++ 就会变成死循环。
 *   累加的和要先设为 0，累乘的积要先设为 1。
 */
#include <stdio.h>

int main(void)
{
    int i = 0;
    while (i < 10) {
        printf("我第%d天吃了%d个包子\n", i, i);
        i++;
    }
    printf("包子循环结束后 i = %d\n", i);

    i = 7;
    while (i <= 15) {
        printf("%d ", i);
        i++;
    }
    printf("\n7 到 15 结束后 i = %d\n", i);

    i = 53;
    while (i >= 48) {
        printf("%d ", i);
        i--;
    }
    printf("\n倒序结束后 i = %d\n", i);

    printf("10 以内的偶数: ");
    i = 0;
    while (i <= 10) {
        printf("%d ", i);
        i += 2;
    }
    printf("\n");

    i = 0;
    int sum = 0;
    while (i <= 100) {
        sum += i;
        i++;
    }
    printf("0 到 100 的和 = %d\n", sum);

    i = 1;
    int product = 1;
    while (i < 10) {
        product *= i;
        i += 2;
    }
    printf("10 以内奇数的乘积 = %d\n", product);
    return 0;
}
