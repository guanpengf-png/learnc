/*
 * 章节：函数练习
 * 知识点：
 *   判断质数：小于等于 1 的都不是。从 2 试到 n / 2，发现因数就可以结束。
 *   某年的第几天：先按平年月份天数累加，闰年再把二月改成 29。
 *     闰年规则：能被 4 整除但不能被 100 整除，或者能被 400 整除。
 *   在数组里找一个值：找到返回下标，找不到返回 -1。
 *   实心三角形和空心金字塔：外层决定行，内层先打空格再打星号。
 *     第 i 行（从 1 计数）的空格数是 总行数 - i，星号数是 2 * i - 1。
 *     空心时，只有第一列、最后一列和最后一行打星号。
 */
#include <stdio.h>
#include <stdbool.h>

static bool is_prime(int n)
{
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

static int day_of_year(int year, int month, int day)
{
    int days_before[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap) {
        days_before[2] = 29;
    }
    int sum = day;
    for (int m = 1; m < month; m++) {
        sum += days_before[m];
    }
    return sum;
}

static int find_index(const int arr[], int len, int target)
{
    for (int i = 0; i < len; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

static void print_solid(int rows)
{
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= rows - i; j++) {
            printf(" ");
        }
        for (int j = 1; j <= 2 * i - 1; j++) {
            printf("*");
        }
        printf("\n");
    }
}

static void print_hollow(int rows)
{
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= rows - i; j++) {
            printf(" ");
        }
        for (int j = 1; j <= 2 * i - 1; j++) {
            if (i == rows || j == 1 || j == 2 * i - 1) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main(void)
{
    printf("1 %s质数, 2 %s质数, 9 %s质数, 97 %s质数\n",
           is_prime(1) ? "是" : "不是",
           is_prime(2) ? "是" : "不是",
           is_prime(9) ? "是" : "不是",
           is_prime(97) ? "是" : "不是");
    printf("2024-03-01 是第 %d 天\n", day_of_year(2024, 3, 1));
    printf("2023-03-01 是第 %d 天\n", day_of_year(2023, 3, 1));

    int arr[] = {4, 7, 1, 9, 3};
    int len = (int)(sizeof arr / sizeof arr[0]);
    printf("9 的下标 = %d\n", find_index(arr, len, 9));
    printf("8 的下标 = %d\n", find_index(arr, len, 8));

    printf("实心:\n");
    print_solid(5);
    printf("空心:\n");
    print_hollow(5);
    return 0;
}
