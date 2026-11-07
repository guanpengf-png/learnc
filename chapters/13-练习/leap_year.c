/*
 * 练习：判断闰年
 * 知识点：
 *   能被 4 整除但不能被 100 整除，是闰年。
 *   能被 400 整除，也是闰年。1900 不是，2000 是。
 *   两个条件用 || 连接。% 用来判断整除，余数是 0 才算整除。
 */
#include <stdio.h>
#include <stdbool.h>

static bool is_leap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static void report(int year)
{
    printf("%d 是%s\n", year, is_leap(year) ? "闰年" : "平年");
}

int main(void)
{
    report(1900);
    report(2000);
    report(2023);
    report(2024);
    return 0;
}
