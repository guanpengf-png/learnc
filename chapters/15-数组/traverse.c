/*
 * 章节：遍历数组
 * 知识点：
 *   遍历就是按顺序碰到每一个元素。下标从 0 走到 长度 - 1，用 for 最直接。
 *   循环变量同时可以用来赋值。倒序时从 长度 - 1 走到 0，判断条件用 >= 0。
 */
#include <stdio.h>

int main(void)
{
    int arr[5] = {100, 200, 300, 400, 500};
    int arr_len = (int)(sizeof arr / sizeof arr[0]);

    for (int i = 0; i < arr_len; i++) {
        printf("下标 %d, 元素 %d\n", i, arr[i]);
    }

    int seq[10];
    int seq_len = (int)(sizeof seq / sizeof seq[0]);
    for (int i = 0; i < seq_len; i++) {
        seq[i] = i;
    }
    printf("倒序: ");
    for (int i = seq_len - 1; i >= 0; i--) {
        printf("%d ", seq[i]);
    }
    printf("\n");
    return 0;
}
