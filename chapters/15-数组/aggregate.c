/*
 * 章节：数组求和、平均数、最大最小值
 * 知识点：
 *   求和：先让 sum = 0，再把每个元素加进去。
 *   平均数：和要乘 1.0 或写成浮点除法，否则两个 int 相除会丢掉小数。
 *   最大最小值：先假设 arr[0] 既是最大也是最小，再和后面的元素比。数组不能是空的。
 *   求最小值的下标时，同样先记下 0。有多个相同最小值时，这里保留最先遇到的那个下标。
 *   讲义把 {1..10} 的结果印成“最大值 1，最小值 10”，和代码对不上。正确结果是最大值 10，最小值 1。
 */
#include <stdio.h>

int main(void)
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int len = (int)(sizeof arr / sizeof arr[0]);
    int sum = 0;

    for (int i = 0; i < len; i++) {
        sum += arr[i];
    }
    double avg = sum * 1.0 / len;
    printf("和 = %d, 平均数 = %.2f\n", sum, avg);

    int max = arr[0];
    int min = arr[0];
    int min_index = 0;
    for (int i = 1; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
        if (arr[i] < min) {
            min = arr[i];
            min_index = i;
        }
    }
    printf("最大值 = %d, 最小值 = %d, 最小值下标 = %d\n", max, min, min_index);

    int values[] = {8, 3, 3, 9, 1, 1};
    int n = (int)(sizeof values / sizeof values[0]);
    int first_min = 0;
    for (int i = 1; i < n; i++) {
        if (values[i] < values[first_min]) {
            first_min = i;
        }
    }
    printf("另一个数组的最小值 = %d, 第一次出现的下标 = %d\n", values[first_min], first_min);
    return 0;
}
