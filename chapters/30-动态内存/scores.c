/*
 * 章节：动态数组保存成绩
 * 知识点：
 *   元素个数要到运行时才知道时，用 malloc 按个数申请，而不是写一个固定长度的数组。
 *   检测函数只拿指针和长度。动态数组没有自己的长度信息，长度必须另外传。
 *   用完释放。这里用一组现成的成绩代替键盘输入。
 */
#include <stdio.h>
#include <stdlib.h>

static void show_failed(const int *scores, int len)
{
    for (int i = 0; i < len; i++) {
        if (scores[i] < 60) {
            printf("%d\n", scores[i]);
        }
    }
}

int main(void)
{
    int sample[] = {80, 45, 90, 59, 60};
    int len = (int)(sizeof sample / sizeof sample[0]);
    int *scores = malloc((size_t)len * sizeof *scores);
    if (scores == NULL) {
        printf("分配空间失败\n");
        return 1;
    }
    for (int i = 0; i < len; i++) {
        scores[i] = sample[i];
    }
    printf("不及格:\n");
    show_failed(scores, len);
    free(scores);
    return 0;
}
