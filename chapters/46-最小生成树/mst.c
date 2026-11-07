/*
 * 章节：最小生成树（Prim 与 Kruskal）
 * 知识点：
 *   生成树：连通无向图里，选出 V-1 条边把所有顶点连起来，且不形成环。
 *   最小生成树（MST）：所有生成树里，边权总和最小的那棵。
 *   生活里的理解：要给几个城市修路，保证任意两个城市能互相到达，怎样花最少的钱。
 *
 *   两个经典算法都基于同一个思想——切分性质：
 *     把顶点分成两堆，跨过这两堆的边里，权值最小的那条一定属于某棵最小生成树。
 *   所以两个算法只是“怎么挑这条最小边”的做法不同。
 *
 *   Prim（加点法）：
 *     从任意一个点开始，维护一棵已经连好的树。
 *     每次从“树内 -> 树外”的边里挑最小的一条，把新点拉进树。
 *     朴素写法 O(V^2)，适合稠密图。
 *
 *   Kruskal（加边法）：
 *     把所有边按权值从小到大排序，依次尝试加入。
 *     如果这条边的两个端点已经连通（加进去会成环），就丢掉。
 *     否则加入，直到选够 V-1 条边。
 *     需要并查集判断“是否已经连通”，排序 O(E log E)，适合稀疏图。
 *
 *   并查集（Union-Find）：
 *     parent[x] 指向 x 的父结点，根结点指向自己。
 *     find 一直往上找到根；union 把一个根挂到另一个根下面。
 *     路径压缩：查找时把路上的结点都直接挂到根上，之后接近 O(1)。
 *     按秩合并：矮的树挂到高的树下面，避免树变高。
 *
 *   应用：网络布线、修路、电网、水管、聚类（切掉权值最大的边）、图像分割。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 6
#define MAX_EDGES 32
#define INF 1000000

typedef struct {
    int matrix[MAX_VERTICES][MAX_VERTICES];   /* 0 表示没有边 */
    int vertex_count;
} Graph;

typedef struct {
    int u;
    int v;
    int weight;
} Edge;

/* ---------------- 并查集 ---------------- */
typedef struct {
    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES];
} UnionFind;

static void uf_init(UnionFind *uf, int n)
{
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
}

/* 路径压缩：递归上去，回来时把每个结点直接挂到根 */
static int uf_find(UnionFind *uf, int x)
{
    if (uf->parent[x] != x) {
        uf->parent[x] = uf_find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

/* 按秩合并；返回 false 表示本来就在同一棵树里（会成环） */
static bool uf_union(UnionFind *uf, int a, int b)
{
    int root_a = uf_find(uf, a);
    int root_b = uf_find(uf, b);

    if (root_a == root_b) {
        return false;
    }
    if (uf->rank[root_a] < uf->rank[root_b]) {
        int temp = root_a;
        root_a = root_b;
        root_b = temp;
    }
    uf->parent[root_b] = root_a;
    if (uf->rank[root_a] == uf->rank[root_b]) {
        uf->rank[root_a]++;
    }
    return true;
}

/* ---------------- 图的基本操作 ---------------- */
static void init_graph(Graph *graph, int vertex_count)
{
    graph->vertex_count = vertex_count;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph->matrix[i][j] = 0;
        }
    }
}

static void add_edge(Graph *graph, int u, int v, int weight)
{
    graph->matrix[u][v] = weight;
    graph->matrix[v][u] = weight;
}

/* ---------------- Prim ---------------- */
static void prim(const Graph *graph, int start)
{
    int key[MAX_VERTICES];
    int parent[MAX_VERTICES];
    bool in_mst[MAX_VERTICES] = {false};

    for (int i = 0; i < graph->vertex_count; i++) {
        key[i] = INF;
        parent[i] = -1;
    }
    key[start] = 0;

    int total = 0;

    printf("Prim 选边: ");
    for (int step = 0; step < graph->vertex_count; step++) {
        int u = -1;
        for (int i = 0; i < graph->vertex_count; i++) {
            if (!in_mst[i] && (u == -1 || key[i] < key[u])) {
                u = i;
            }
        }
        if (u == -1) {
            break;
        }

        in_mst[u] = true;
        if (parent[u] != -1) {
            printf("%d-%d(%d) ", parent[u], u, key[u]);
            total += key[u];
        }

        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->matrix[u][v] != 0 && !in_mst[v] && graph->matrix[u][v] < key[v]) {
                key[v] = graph->matrix[u][v];
                parent[v] = u;
            }
        }
    }
    printf("\nPrim 总权重: %d\n", total);
}

/* ---------------- Kruskal ---------------- */
static int compare_edges(const void *a, const void *b)
{
    const Edge *edge_a = a;
    const Edge *edge_b = b;
    return edge_a->weight - edge_b->weight;
}

static void kruskal(const Graph *graph, Edge *edges, int edge_count)
{
    UnionFind uf;
    uf_init(&uf, graph->vertex_count);

    qsort(edges, (size_t)edge_count, sizeof *edges, compare_edges);

    int total = 0;
    int used = 0;

    printf("Kruskal 选边（按权值从小到大）: ");
    for (int i = 0; i < edge_count && used < graph->vertex_count - 1; i++) {
        if (uf_union(&uf, edges[i].u, edges[i].v)) {
            printf("%d-%d(%d) ", edges[i].u, edges[i].v, edges[i].weight);
            total += edges[i].weight;
            used++;
        }
        /* 否则两个端点已经连通，加了会形成环，直接跳过 */
    }
    printf("\nKruskal 总权重: %d，共 %d 条边\n", total, used);
}

int main(void)
{
    /*
     * 6 个顶点，10 条边：
     *   0-1 6   0-2 1   0-3 5
     *   1-2 5   1-4 3
     *   2-3 5   2-4 6   2-5 4
     *   3-5 2
     *   4-5 6
     */
    Graph graph;
    init_graph(&graph, 6);
    add_edge(&graph, 0, 1, 6);
    add_edge(&graph, 0, 2, 1);
    add_edge(&graph, 0, 3, 5);
    add_edge(&graph, 1, 2, 5);
    add_edge(&graph, 1, 4, 3);
    add_edge(&graph, 2, 3, 5);
    add_edge(&graph, 2, 4, 6);
    add_edge(&graph, 2, 5, 4);
    add_edge(&graph, 3, 5, 2);
    add_edge(&graph, 4, 5, 6);

    Edge edges[] = {
        {0, 1, 6}, {0, 2, 1}, {0, 3, 5},
        {1, 2, 5}, {1, 4, 3},
        {2, 3, 5}, {2, 4, 6}, {2, 5, 4},
        {3, 5, 2}, {4, 5, 6}
    };
    int edge_count = (int)(sizeof edges / sizeof edges[0]);

    prim(&graph, 0);
    printf("\n");
    kruskal(&graph, edges, edge_count);

    printf("\n两种算法得到的总权重应该相同（15），选出的边可能不同。\n");
    printf("Prim 适合稠密图 O(V^2)，Kruskal 适合稀疏图 O(E log E)。\n");
    return 0;
}
