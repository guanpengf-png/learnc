/*
 * 课后习题答案
 *
 * 题目：
 *   1. 判断一个年份是否为闰年。
 *   2. 判断一个整数是否是水仙花数（3 位数，各位数字立方和等于本身）。
 *   3. 根据输入的年份和月份，输出该月天数（考虑闰年 2 月 29 天）。
 *   4. 输出小写 a-z 以及大写 Z-A。
 *   5. 求 1 - 1/2 + 1/3 - 1/4 + ... + 1/100 的和。
 *
 * 编译：clang -Wall -Wextra -O2 answer.c -o answer
 * 运行：./answer
 */

#include <stdbool.h>
#include <stdio.h>

/* ============================================================
 * 第 1 题：判断闰年
 *
 * 规则：
 *   能被 4 整除，但不能被 100 整除的是闰年；
 *   或者能被 400 整除的也是闰年。
 * 例如：2000 是闰年，1900 不是，2024 是。
 * ============================================================ */
static bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/* ============================================================
 * 第 2 题：判断水仙花数
 *
 * 水仙花数是 3 位数，且各位数字的立方和等于它本身。
 * 例如 153 = 1^3 + 5^3 + 3^3。
 * ============================================================ */
static bool is_narcissistic(int number)
{
    if (number < 100 || number > 999) {
        return false;               /* 题目限定是 3 位数 */
    }

    int hundreds = number / 100;        /* 百位 */
    int tens = number / 10 % 10;        /* 十位 */
    int ones = number % 10;             /* 个位 */

    return hundreds * hundreds * hundreds
         + tens * tens * tens
         + ones * ones * ones == number;
}

/* ============================================================
 * 第 3 题：根据年份和月份求当月天数
 *
 * 大月 31 天：1、3、5、7、8、10、12
 * 小月 30 天：4、6、9、11
 * 2 月：闰年 29 天，平年 28 天
 * 月份非法返回 -1。
 * ============================================================ */
static int days_in_month(int year, int month)
{
    switch (month) {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return is_leap_year(year) ? 29 : 28;
        default:
            return -1;              /* 月份不在 1-12 之间 */
    }
}

/* ============================================================
 * 第 4 题：输出小写 a-z 和大写 Z-A
 * ============================================================ */
static void print_letters(void)
{
    printf("小写 a-z: ");
    for (char ch = 'a'; ch <= 'z'; ch++) {
        putchar(ch);
    }
    printf("\n");

    printf("大写 Z-A: ");
    for (char ch = 'Z'; ch >= 'A'; ch--) {
        putchar(ch);
    }
    printf("\n");
}

/* ============================================================
 * 第 5 题：求 1 - 1/2 + 1/3 - 1/4 + ... + 1/100
 *
 * 注意：一定要写 1.0 / i，写 1 / i 会做整数除法，
 * 除了 i = 1 以外结果全是 0。
 * 奇数项取正号，偶数项取负号。
 * ============================================================ */
static double alternating_sum(int n)
{
    double sum = 0.0;

    for (int i = 1; i <= n; i++) {
        double term = 1.0 / i;

        if (i % 2 == 0) {
            sum -= term;            /* 偶数项是减 */
        } else {
            sum += term;            /* 奇数项是加 */
        }
    }
    return sum;
}

int main(void)
{
    /* ---------------- 第 1 题 ---------------- */
    printf("========== 第 1 题：判断闰年 ==========\n");
    int years[] = {2000, 1900, 2024, 2023};
    for (int i = 0; i < 4; i++) {
        printf("%d 年：%s\n", years[i], is_leap_year(years[i]) ? "闰年" : "平年");
    }

    /* ---------------- 第 2 题 ---------------- */
    printf("\n========== 第 2 题：水仙花数 ==========\n");
    printf("100 ~ 999 之间的水仙花数有：");
    for (int number = 100; number <= 999; number++) {
        if (is_narcissistic(number)) {
            printf("%d ", number);
        }
    }
    printf("\n");
    printf("单独判断 153：%s\n", is_narcissistic(153) ? "是水仙花数" : "不是");
    printf("单独判断 123：%s\n", is_narcissistic(123) ? "是水仙花数" : "不是");

    /* ---------------- 第 3 题 ---------------- */
    printf("\n========== 第 3 题：求某月天数 ==========\n");
    int year, month;
    printf("请输入年份和月份（例如 2024 2）：");
    fflush(stdout);

    if (scanf("%d %d", &year, &month) != 2) {
        printf("输入格式错误\n");
    } else {
        int days = days_in_month(year, month);
        if (days < 0) {
            printf("月份必须在 1-12 之间\n");
        } else {
            printf("%d 年 %d 月有 %d 天\n", year, month, days);
        }
    }

    /* ---------------- 第 4 题 ---------------- */
    printf("\n========== 第 4 题：输出字母 ==========\n");
    print_letters();

    /* ---------------- 第 5 题 ---------------- */
    printf("\n========== 第 5 题：交错级数求和 ==========\n");
    printf("1 - 1/2 + 1/3 - ... - 1/100 = %.6f\n", alternating_sum(100));

    return 0;
}
