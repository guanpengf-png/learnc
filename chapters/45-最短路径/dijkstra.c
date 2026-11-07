/*
 * 章节：最短路径（Dijkstra 算法）
 * 知识点：
 *   带权图里，每条边有代价（距离、时间、费用）。最短路径就是总代价最小的那条路。
 *   Dijkstra 适合“边权非负”的图，核心动作叫松弛（relax）：
 *     如果 dist[u] + w(u,v) < dist[v]，就说明从 u 绕一下更近，更新 dist[v]。
 *   算法流程：
 *     1. dist[起点] = 0，其余全部设成无穷大。
 *     2. 每次挑出“还没确定、dist 最小”的顶点 u，把它标记为已确定。
 *     3. 用 u 去松弛它的所有邻居。
 *     4. 重复，直到所有点确定或剩下的点都到不了。
 *   为什么挑最小的就能确定？因为边权非负，后面再绕路只会更大，不可能更小。
 *   这就是贪心。也正因为这个前提，有负权边的图不能用 Dijkstra（要用 Bellman-Ford）。
 *   朴素写法每次线性扫描找最小值：O(V^2)。
 *   用二叉堆/优先队列维护 dist：O((V + E) log V)，适合稀疏大图。
 *   应用：地图导航、网络路由（OSPF）、游戏寻路、物流调度、汇率套利（取对数后找负环，属于变体）。
 */
#include <stdbool.h>
#include <stdio.h>

#define MAX_VERTICES 6
#define INF 1000000

typedef struct {
    int matrix[MAX_VERTICES][MAX_VERTICES];   /* 0 表示没有边 */
    int vertex_count;
} Graph;

static void init_graph(Graph *graph, int vertex_count)
{
    graph->vertex_count = vertex_count;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph->matrix[i][j] = 0;
        }
    }
}

/* 无向带权图 */
static void add_edge(Graph *graph, int u, int v, int weight)
{
    graph->matrix[u][v] = weight;
    graph->matrix[v][u] = weight;
}

static void dijkstra(const Graph *graph, int start, int *dist, int *prev)
{
    bool visited[MAX_VERTICES] = {false};

    for (int i = 0; i < graph->vertex_count; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (int step = 0; step < graph->vertex_count; step++) {
        /* 1. 找还没确定、dist 最小的顶点 */
        int u = -1;
        for (int i = 0; i < graph->vertex_count; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }
        if (u == -1 || dist[u] == INF) {
            break;      /* 剩下的点都到不了 */
        }
        visited[u] = true;

        /* 2. 用 u 松弛邻居 */
        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->matrix[u][v] == 0) {
                continue;
            }
            int candidate = dist[u] + graph->matrix[u][v];
            if (candidate < dist[v]) {
                dist[v] = candidate;
                prev[v] = u;
            }
        }
    }
}

/* 顺着 prev 往回走，再倒着打印 */
static void print_path(const int *prev, int target)
{
    int stack[MAX_VERTICES];
    int top = 0;

    for (int cur = target; cur != -1; cur = prev[cur]) {
        stack[top++] = cur;
    }
    while (top > 0) {
        printf("%d", stack[--top]);
        if (top > 0) {
            printf(" -> ");
        }
    }
}

int main(void)
{
    /*
     * 经典的 6 顶点例子：
     *   0-1 7   0-2 9   0-5 14
     *   1-2 10  1-3 15
     *   2-3 11  2-5 2
     *   3-4 6
     *   4-5 9
     */
    Graph graph;
    init_graph(&graph, 6);
    add_edge(&graph, 0, 1, 7);
    add_edge(&graph, 0, 2, 9);
    add_edge(&graph, 0, 5, 14);
    add_edge(&graph, 1, 2, 10);
    add_edge(&graph, 1, 3, 15);
    add_edge(&graph, 2, 3, 11);
    add_edge(&graph, 2, 5, 2);
    add_edge(&graph, 3, 4, 6);
    add_edge(&graph, 4, 5, 9);

    int dist[MAX_VERTICES];
    int prev[MAX_VERTICES];

    dijkstra(&graph, 0, dist, prev);

    printf("从 0 出发的最短距离:\n");
    for (int i = 0; i < graph.vertex_count; i++) {
        printf("  到 %d: 距离 %d，路径 ", i, dist[i]);
        print_path(prev, i);
        printf("\n");
    }

    printf("\n注意 0->3 不是直接走 1-3（7+15=22），\n");
    printf("而是 0->2->3（9+11=20），说明“走两步”可能比“走一步”更近。\n");

    printf("\n复杂度: 朴素 O(V^2)，优先队列优化 O((V+E) log V)\n");
    printf("限制: 边权不能是负数，否则贪心的“确定”就不成立了\n");
    return 0;
}
