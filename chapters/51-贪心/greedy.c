/*
 * 章节：贪心算法
 * 知识点：
 *   贪心 = 每一步都选当前看起来最好的，选完不后悔、不回头。
 *   它不保证全局最优，只在问题满足“贪心选择性质”时才对。
 *   做题时要回答两个问题：
 *     1. 每一步的“当前最好”是什么？
 *     2. 为什么这么选不会破坏全局最优？（能证明，或者举出反例）
 *
 *   三个经典例子：
 *   1. 活动选择：一堆活动有开始和结束时间，一个人同一时间只能参加一个，
 *      最多能参加几个？
 *      贪心策略：按结束时间从早到晚排序，能参加就参加。
 *      直觉：结束得越早，留给后面活动的时间越多。
 *   2. 分数背包：物品可以切开带走，按“单位重量价值”从高到低装。
 *      每一步都装满最划算的，最后一定最优。
 *   3. 找零：从大面额开始尽量用。对 1、5、10、25 这套面额成立，
 *      但换成 {1, 3, 4} 就不成立了：找 6 元，贪心拿 4+1+1 共 3 枚，
 *      最优是 3+3 共 2 枚。
 *
 *   贪心和动态规划的区别：贪心做完选择就不回头；DP 会保留所有可能的状态。
 *   0-1 背包（物品不能切）不能用贪心，必须用 DP。
 *   应用：活动安排、霍夫曼编码、最小生成树（Prim/Kruskal）、Dijkstra、任务调度、找零。
 */
#include <stdio.h>
#include <stdlib.h>

/* ---------------- 1. 活动选择 ---------------- */
typedef struct {
    int start;
    int end;
} Activity;

static int compare_by_end(const void *a, const void *b)
{
    const Activity *act_a = a;
    const Activity *act_b = b;
    return act_a->end - act_b->end;
}

static int select_activities(Activity *activities, int n)
{
    qsort(activities, (size_t)n, sizeof *activities, compare_by_end);

    int count = 0;
    int last_end = -1;

    printf("选中的活动: ");
    for (int i = 0; i < n; i++) {
        if (activities[i].start >= last_end) {
            printf("[%d,%d] ", activities[i].start, activities[i].end);
            last_end = activities[i].end;
            count++;
        }
        /* 否则和上一个选中的活动时间冲突，跳过 */
    }
    printf("\n");
    return count;
}

/* ---------------- 2. 分数背包 ---------------- */
typedef struct {
    int weight;
    int value;
} Item;

static int compare_by_ratio(const void *a, const void *b)
{
    const Item *item_a = a;
    const Item *item_b = b;

    double ratio_a = (double)item_a->value / item_a->weight;
    double ratio_b = (double)item_b->value / item_b->weight;

    if (ratio_a < ratio_b) {
        return 1;      /* 单位价值高的排前面 */
    }
    if (ratio_a > ratio_b) {
        return -1;
    }
    return 0;
}

static double fractional_knapsack(Item *items, int n, double capacity)
{
    qsort(items, (size_t)n, sizeof *items, compare_by_ratio);

    double total = 0.0;

    for (int i = 0; i < n && capacity > 0; i++) {
        if ((double)items[i].weight <= capacity) {
            total += items[i].value;
            capacity -= items[i].weight;
            printf("  整个拿走 %dkg，价值 %d\n", items[i].weight, items[i].value);
        } else {
            double fraction = capacity / items[i].weight;
            total += items[i].value * fraction;
            printf("  只拿 %.1fkg（%.0f%%），价值 %.1f\n",
                   capacity, fraction * 100, items[i].value * fraction);
            capacity = 0;
        }
    }
    return total;
}

/* ---------------- 3. 找零 ---------------- */
static int greedy_coins(const int *coins, int n, int amount)
{
    int count = 0;

    printf("  找零 %d 元: ", amount);
    for (int i = 0; i < n; i++) {
        while (amount >= coins[i]) {
            amount -= coins[i];
            count++;
            printf("%d ", coins[i]);
        }
    }
    printf("-> 共 %d 枚\n", count);
    return count;
}

int main(void)
{
    /* 1. 活动选择 */
    Activity activities[] = {
        {1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 9}, {5, 9},
        {6, 10}, {8, 11}, {8, 12}, {2, 14}, {12, 16}
    };
    int activity_count = (int)(sizeof activities / sizeof activities[0]);

    printf("活动选择（按结束时间贪心）:\n");
    int selected = select_activities(activities, activity_count);
    printf("最多能参加 %d 个活动\n\n", selected);

    /* 2. 分数背包 */
    Item items[] = {{10, 60}, {20, 100}, {30, 120}};
    int item_count = (int)(sizeof items / sizeof items[0]);

    printf("分数背包（容量 50kg，可以切分）:\n");
    double best = fractional_knapsack(items, item_count, 50.0);
    printf("最大价值 %.0f\n\n", best);

    /* 3. 找零 */
    printf("找零（贪心）:\n");
    int coins[] = {25, 10, 5, 1};
    greedy_coins(coins, (int)(sizeof coins / sizeof coins[0]), 63);

    printf("\n反例：面额 {4, 3, 1}，找 6 元\n");
    int bad_coins[] = {4, 3, 1};
    greedy_coins(bad_coins, 3, 6);
    printf("  但最优是 3 + 3，只要 2 枚 —— 所以贪心不是万能的\n");

    printf("\n0-1 背包（物品不能切）也不能贪心，要用第 47 章的动态规划。\n");
    return 0;
}
