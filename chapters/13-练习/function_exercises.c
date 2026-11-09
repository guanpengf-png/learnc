/*
 * 课后练习题（函数篇）
 *
 * 题目：
 *   1. 定义函数，判断一个数字是否是质数。
 *   2. 定义函数，输入年月日，求这一天是当年的第几天。
 *   3. 找某个数据在数组中是否存在，存在则返回索引，否则返回 -1。
 *   4. 定义函数，输出实心三角形。
 *
 * 编译：clang -Wall -Wextra -O2 function_exercises.c -o function_exercises
 * 运行：./function_exercises
 */

#include <stdbool.h>
#include <stdio.h>

/* ============================================================
 * 第 1 题：判断质数
 *
 * 质数：大于 1，且只能被 1 和它本身整除的整数。
 * 2 是最小的质数，也是唯一的偶质数。
 *
 * 只需要试除到 sqrt(n)：如果 n 有因数，一定有一对因数
 * 一个 <= sqrt(n)，一个 >= sqrt(n)。
 * 写成 i * i <= n 可以避免浮点开方和溢出。
 * ============================================================ */
static bool is_prime(int number)
{
    if (number < 2) {
        return false;               /* 0、1 和负数都不是质数 */
    }
    if (number == 2) {
        return true;
    }
    if (number % 2 == 0) {
        return false;               /* 大于 2 的偶数一定不是质数 */
    }

    for (int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

/* ============================================================
 * 第 2 题：年月日是当年的第几天
 * ============================================================ */

static bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/*
 * 返回 -1 表示日期非法。
 * 思路：先把前 month-1 个月的天数加起来，再加上 day；
 *       如果 month > 2 且是闰年，再加 1 天（2 月多出来的一天）。
 */
static int day_of_year(int year, int month, int day)
{
    int days_per_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12 || day < 1) {
        return -1;
    }

    int max_day = days_per_month[month - 1];
    if (month == 2 && is_leap_year(year)) {
        max_day = 29;
    }
    if (day > max_day) {
        return -1;
    }

    int total = day;
    for (int i = 0; i < month - 1; i++) {
        total += days_per_month[i];
    }

    if (month > 2 && is_leap_year(year)) {
        total += 1;
    }
    return total;
}

/* ============================================================
 * 第 3 题：在数组中查找数据
 *
 * 返回下标；找不到返回 -1。
 * 线性查找，O(n)。如果数组有序，可以改成二分查找。
 * ============================================================ */
static int find_index(const int *array, int length, int key)
{
    for (int i = 0; i < length; i++) {
        if (array[i] == key) {
            return i;
        }
    }
    return -1;
}

/* ============================================================
 * 第 4 题：输出实心三角形
 *
 * 第 i 行（从 1 开始）：
 *   先输出 rows - i 个空格，
 *   再输出 2 * i - 1 个星号。
 * ============================================================ */
static void print_triangle(int rows)
{
    for (int i = 1; i <= rows; i++) {
        for (int space = 0; space < rows - i; space++) {
            putchar(' ');
        }
        for (int star = 0; star < 2 * i - 1; star++) {
            putchar('*');
        }
        putchar('\n');
    }
}

int main(void)
{
    /* ---------------- 第 1 题 ---------------- */
    printf("========== 第 1 题：判断质数 ==========\n");
    int numbers[] = {1, 2, 3, 4, 17, 18, 97, 100};
    int count = (int)(sizeof numbers / sizeof numbers[0]);

    for (int i = 0; i < count; i++) {
        printf("%3d：%s\n", numbers[i], is_prime(numbers[i]) ? "是质数" : "不是质数");
    }

    printf("1~50 之间的质数：");
    for (int n = 1; n <= 50; n++) {
        if (is_prime(n)) {
            printf("%d ", n);
        }
    }
    printf("\n");

    /* ---------------- 第 2 题 ---------------- */
    printf("\n========== 第 2 题：当年的第几天 ==========\n");
    printf("2024-03-01 是 2024 年的第 %d 天\n", day_of_year(2024, 3, 1));
    printf("2023-03-01 是 2023 年的第 %d 天\n", day_of_year(2023, 3, 1));
    printf("2024-12-31 是 2024 年的第 %d 天\n", day_of_year(2024, 12, 31));
    printf("2023-12-31 是 2023 年的第 %d 天\n", day_of_year(2023, 12, 31));
    printf("2024-02-30 的结果：%d（-1 表示日期非法）\n", day_of_year(2024, 2, 30));

    /* ---------------- 第 3 题 ---------------- */
    printf("\n========== 第 3 题：数组查找 ==========\n");
    int array[] = {10, 20, 30, 40, 50};
    int length = (int)(sizeof array / sizeof array[0]);

    printf("数组：");
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    int keys[] = {30, 50, 99};
    for (int i = 0; i < 3; i++) {
        int index = find_index(array, length, keys[i]);
        if (index == -1) {
            printf("查找 %d：不存在，返回 -1\n", keys[i]);
        } else {
            printf("查找 %d：存在，下标为 %d\n", keys[i], index);
        }
    }

    /* ---------------- 第 4 题 ---------------- */
    printf("\n========== 第 4 题：实心三角形 ==========\n");
    print_triangle(5);

    return 0;
}
