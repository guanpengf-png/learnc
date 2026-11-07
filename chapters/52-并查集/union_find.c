/*
 * 章节：并查集（Union-Find / Disjoint Set）
 * 知识点：
 *   并查集用来维护一堆元素的分组关系，只做两件事：
 *     find(x)：查 x 属于哪个组，返回这一组的代表（根结点）
 *     union(a, b)：把 a 和 b 所在的组合并成一个
 *   实现：用数组 parent 表示一片森林。
 *     parent[x] 是 x 的父结点，根结点的 parent 指向自己。
 *     同一组的元素最终都能找到同一个根。
 *   两个关键优化：
 *     路径压缩：find 的时候，把路上的结点全部直接挂到根下面。
 *               以后再查这些点就是一步到位，树被压得很扁。
 *     按秩合并：union 时把矮的树挂到高的树下面，避免树越连越高。
 *   两个优化一起用，单次操作的平均复杂度接近 O(1)
 *   （严格说是反阿克曼函数 α(n)，在现实数据规模下不超过 4）。
 *   应用：网络连通性、朋友圈/账户合并、Kruskal 最小生成树、图片连通区域、判断无向图有没有环。
 */
#include <stdbool.h>
#include <stdio.h>

#define MAX_N 10

typedef struct {
    int parent[MAX_N];
    int rank[MAX_N];     /* 树的高度（近似），用来做按秩合并 */
    int count;           /* 当前有多少个组 */
} UnionFind;

static void uf_init(UnionFind *uf, int n)
{
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;   /* 一开始每个人自成一个组 */
        uf->rank[i] = 0;
    }
    uf->count = n;
}

/* 找根。递归回来时顺手做路径压缩 */
static int uf_find(UnionFind *uf, int x)
{
    if (uf->parent[x] != x) {
        uf->parent[x] = uf_find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

/* 合并两个组；返回 false 表示本来就在同一组 */
static bool uf_union(UnionFind *uf, int a, int b)
{
    int root_a = uf_find(uf, a);
    int root_b = uf_find(uf, b);

    if (root_a == root_b) {
        return false;
    }

    /* 按秩合并：矮的挂到高的下面 */
    if (uf->rank[root_a] < uf->rank[root_b]) {
        int temp = root_a;
        root_a = root_b;
        root_b = temp;
    }
    uf->parent[root_b] = root_a;

    if (uf->rank[root_a] == uf->rank[root_b]) {
        uf->rank[root_a]++;
    }

    uf->count--;
    return true;
}

static bool uf_connected(UnionFind *uf, int a, int b)
{
    return uf_find(uf, a) == uf_find(uf, b);
}

static void print_groups(UnionFind *uf, int n)
{
    printf("  分组: ");
    for (int i = 0; i < n; i++) {
        printf("%d->%d ", i, uf_find(uf, i));
    }
    printf("\n  共有 %d 组\n", uf->count);
}

int main(void)
{
    UnionFind uf;
    uf_init(&uf, MAX_N);

    printf("初始状态（10 个人，各自一组）:\n");
    print_groups(&uf, MAX_N);

    /* 建立关系：0-1、1-2、3-4、5-6、6-7、7-8 */
    int pairs[][2] = {
        {0, 1}, {1, 2},
        {3, 4},
        {5, 6}, {6, 7}, {7, 8}
    };
    int pair_count = (int)(sizeof pairs / sizeof pairs[0]);

    for (int i = 0; i < pair_count; i++) {
        uf_union(&uf, pairs[i][0], pairs[i][1]);
    }

    printf("\n建立 6 条关系之后:\n");
    print_groups(&uf, MAX_N);

    printf("\n连通性查询:\n");
    printf("  0 和 2: %s\n", uf_connected(&uf, 0, 2) ? "连通" : "不连通");
    printf("  0 和 3: %s\n", uf_connected(&uf, 0, 3) ? "连通" : "不连通");
    printf("  5 和 8: %s\n", uf_connected(&uf, 5, 8) ? "连通" : "不连通");

    printf("\n合并 2 和 4（两组连起来）:\n");
    uf_union(&uf, 2, 4);
    print_groups(&uf, MAX_N);

    printf("\n再查 0 和 3: %s\n", uf_connected(&uf, 0, 3) ? "连通" : "不连通");

    printf("\nKruskal 算法就是靠并查集判断“加这条边会不会成环”。\n");
    printf("两个点已经连通，再加边就会形成环，直接跳过。\n");
    return 0;
}
