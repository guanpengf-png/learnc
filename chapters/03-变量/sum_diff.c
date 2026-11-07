/*
 * 章节：课堂案例
 * 知识点：输入两个整数，用变量保存，再做加法和减法。
 * 表达式 a + b、a - b 的结果可以直接交给 printf，不必都先存进变量。
 *
 * 试运行输入：10 4
 * 期望输出：a和b的和为:14,差为:6
 */
#include <stdio.h>

int main(void)
{
    int a, b;
    printf("请您输入两个数字,并使用空格隔开\n");
    if (scanf("%d %d", &a, &b) != 2) {
        return 1;
    }

    int sum = a + b;
    printf("a和b的和为:%d,差为:%d\n", sum, a - b);
    return 0;
}
