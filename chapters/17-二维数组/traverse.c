/*
 * 章节：二维数组的遍历和内存顺序
 * 知识点：
 *   行数 = sizeof 整个数组 / sizeof 一行。
 *   列数 = sizeof 一行 / sizeof 一个元素。
 *   遍历用两层循环：外层行走，内层走列。
 *   画成表格只是逻辑上的看法。内存里仍然是一条直线，C 按行存放：
 *   先放完第 0 行的每一列，再放第 1 行。
 */
#include <stdio.h>

int main(void)
{
    int arr[2][3] = {
        {10, 20, 30},
        {40, 50, 60}
    };
    int rows = (int)(sizeof arr / sizeof arr[0]);
    int cols = (int)(sizeof arr[0] / sizeof arr[0][0]);
    int sum = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", arr[i][j]);
            sum += arr[i][j];
        }
        printf("\n");
    }
    printf("总和 = %d\n", sum);

    printf("按内存顺序: ");
    int *flat = &arr[0][0];
    for (int i = 0; i < rows * cols; i++) {
        printf("%d ", flat[i]);
    }
    printf("\n");
    return 0;
}
