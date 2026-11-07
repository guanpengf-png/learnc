/*
 * 章节：用 break 判断质数
 * 知识点：
 *   质数大于 1，并且除了 1 和它自己以外没有别的因数。
 *   只要发现一个因数，就可以 break，不必继续试。
 *   试到 n / 2 就够了，因为比一半更大的因数一定配了一个更小的因数。
 *   1 不是质数。这个条件要单独处理，否则循环一次都不进，会误判成质数。
 */
#include <stdio.h>
#include <stdbool.h>

static bool is_prime(int number)
{
    if (number <= 1) {
        return false;
    }
    for (int factor = 2; factor <= number / 2; factor++) {
        if (number % factor == 0) {
            return false;
        }
    }
    return true;
}

static void report(int number)
{
    printf("%d%s质数\n", number, is_prime(number) ? "是" : "不是");
}

int main(void)
{
    report(1);
    report(2);
    report(7);
    report(8);
    report(9);
    report(97);
    return 0;
}
