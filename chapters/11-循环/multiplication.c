/*
 * 章节：九九乘法表
 * 知识点：
 *   外层循环控制行，内层循环控制这一行打印几列。
 *   第 i 行打印 i 列，所以内层条件写成 j <= i。
 *   倒三角把外层从 9 减到 1。
 *   右对齐的空白不是空格硬数出来的，而是先打印 (9 - i) 个制表符。
 *   \t 让每一列对齐。
 */
#include <stdio.h>

static void left_increasing(void)
{
    printf("左对齐，行数从上往下增加\n");
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= i; j++) {
            printf("%d*%d=%d\t", j, i, i * j);
        }
        printf("\n");
    }
}

static void left_decreasing(void)
{
    printf("左对齐，行数从上往下减少\n");
    for (int i = 9; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            printf("%d*%d=%d\t", j, i, i * j);
        }
        printf("\n");
    }
}

static void right_increasing(void)
{
    printf("右对齐，行数从上往下增加\n");
    for (int i = 1; i <= 9; i++) {
        for (int blank = 1; blank <= 9 - i; blank++) {
            printf("\t");
        }
        for (int j = 1; j <= i; j++) {
            printf("%d*%d=%d\t", j, i, i * j);
        }
        printf("\n");
    }
}

static void right_decreasing(void)
{
    printf("右对齐，行数从上往下减少\n");
    for (int i = 9; i >= 1; i--) {
        for (int blank = 1; blank <= 9 - i; blank++) {
            printf("\t");
        }
        for (int j = 1; j <= i; j++) {
            printf("%d*%d=%d\t", j, i, i * j);
        }
        printf("\n");
    }
}

int main(void)
{
    left_increasing();
    left_decreasing();
    right_increasing();
    right_decreasing();
    return 0;
}
