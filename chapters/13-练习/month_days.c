/*
 * 练习：按年份和月份求天数
 * 知识点：
 *   把 switch 的穿透和闰年判断合在一起。
 *   大月 31 天，小月 30 天。
 *   2 月：闰年 29 天，平年 28 天。
 */
#include <stdio.h>
#include <stdbool.h>

static bool is_leap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int days_in_month(int year, int month)
{
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return is_leap(year) ? 29 : 28;
    default:
        return 0;
    }
}

static void report(int year, int month)
{
    int days = days_in_month(year, month);
    if (days == 0) {
        printf("%d 年 %d 月不存在\n", year, month);
        return;
    }
    printf("%d 年 %d 月有 %d 天\n", year, month, days);
}

int main(void)
{
    report(2023, 2);
    report(2024, 2);
    report(2024, 4);
    report(2024, 12);
    report(2024, 13);
    return 0;
}
