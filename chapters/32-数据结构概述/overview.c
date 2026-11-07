/*
 * 章节：数据结构是什么
 * 知识点：
 *   沃斯的公式：算法 + 数据结构 = 程序。
 *   数据结构关心两件事：数据元素，以及元素之间的关系。它不关心某个元素里面具体是姓名还是成绩。
 *   逻辑结构是关系本身，和语言无关。四种基本关系：
 *     集合：只是同属一组，没有别的关系。
 *     线性：一对一，有唯一的开头和唯一的结尾。排队就是这种。
 *     树：一对多，有层次和父子关系。家谱、目录都是这种。
 *     图：多对多，由顶点和边组成。铁路网、社交关系都是这种。
 *   习惯上也分成线性结构和非线性结构。栈、队列、数组、链表都是线性结构。
 *   存储结构是逻辑关系在内存里的放法，和语言有关。C 语言里：
 *     顺序存储：逻辑上相邻的元素，物理地址也相邻。一维数组就是这样。
 *     链式存储：逻辑上相邻，物理上可以分开。每个结点除了数据，还保存下一个结点的地址。
 *   索引存储额外做一张目录，查找快，增删时目录也要改。
 *   散列存储用关键字算出地址。讲义没有写两个关键字算出同一地址时怎么办。
 *   运算包括：建立和释放、插入和删除、获取和遍历、修改和排序。
 *   下面只演示两种存储在地址上的差别。索引和散列这一节没有可运行的例程。
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

int main(void)
{
    int arr[4] = {10, 20, 30, 40};
    printf("顺序存储，相邻元素的地址也相邻:\n");
    for (int i = 0; i < 4; i++) {
        printf("  arr[%d] = %d, 地址 %p\n", i, arr[i], (void *)&arr[i]);
    }
    printf("相邻两个 int 相差 %td 个元素\n", &arr[1] - &arr[0]);

    Node *first = malloc(sizeof *first);
    void *gap_a = malloc(64);
    Node *second = malloc(sizeof *second);
    void *gap_b = malloc(64);
    Node *third = malloc(sizeof *third);
    if (first == NULL || second == NULL || third == NULL) {
        free(first);
        free(gap_a);
        free(second);
        free(gap_b);
        free(third);
        return 1;
    }
    first->data = 10;
    second->data = 20;
    third->data = 30;
    first->next = second;
    second->next = third;
    third->next = NULL;

    printf("链式存储，逻辑上相连，地址不必连续:\n");
    for (Node *node = first; node != NULL; node = node->next) {
        printf("  数据 %d, 结点地址 %p, 下一个 %p\n",
               node->data, (void *)node, (void *)node->next);
    }

    free(third);
    free(gap_b);
    free(second);
    free(gap_a);
    free(first);
    return 0;
}
