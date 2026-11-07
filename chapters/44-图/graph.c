/*
 * 章节：图的存储与遍历
 * 知识点：
 *   图由顶点（顶点集合 V）和边（边集合 E）组成，用来表示“多对多”的关系。
 *   有向图：边有方向，A -> B 不代表 B -> A。无向图：边没有方向，A-B 是双向的。
 *   带权图：边上还有数字（距离、费用、时间）。无权图可以看成每条边权重都是 1。
 *   两种常见存法：
 *     邻接矩阵：matrix[i][j] 表示 i 到 j 有没有边（或权重）。
 *               查两点是否相连是 O(1)，但空间固定 O(V^2)，稀疏图很浪费。
 *     邻接表：  每个顶点挂一条链表，存它所有的邻居。
 *               空间 O(V + E)，适合稀疏图；查两点相连要遍历链表。
 *   遍历方式：
 *     BFS（广度优先）：用队列，先访问离起点近的，一圈一圈扩散。
 *                      无权图里第一次到达某个点，走的就是最短路径（边数最少）。
 *     DFS（深度优先）：用递归或栈，一条路走到黑，走不动再回头。
 *                      适合找连通块、判断环、拓扑排序、迷宫问题。
 *   连通分量：无向图里互相能走到的顶点算一块。对每个没访问过的点做一次 BFS/DFS，次数就是分量个数。
 *   应用：社交网络好友关系、地图导航、网页链接、任务依赖、推荐系统、网络连通性检查。
 */
#include <stdbool.h>
#include <stdio.h>

#define MAX_VERTICES 8

typedef struct {
    int matrix[MAX_VERTICES][MAX_VERTICES];
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

/* 无向图：两个方向都要记 */
static void add_edge(Graph *graph, int u, int v)
{
    graph->matrix[u][v] = 1;
    graph->matrix[v][u] = 1;
}

static void print_matrix(const Graph *graph)
{
    printf("邻接矩阵:\n     ");
    for (int j = 0; j < graph->vertex_count; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < graph->vertex_count; j++) {
            printf("%2d ", graph->matrix[i][j]);
        }
        printf("\n");
    }
}

/* 广度优先：队列 + visited，第一次到达就是最短距离（按边数） */
static void bfs(const Graph *graph, int start)
{
    bool visited[MAX_VERTICES] = {false};
    int distance[MAX_VERTICES];
    int queue[MAX_VERTICES];
    size_t head = 0;
    size_t tail = 0;

    for (int i = 0; i < graph->vertex_count; i++) {
        distance[i] = -1;
    }

    visited[start] = true;
    distance[start] = 0;
    queue[tail++] = start;

    printf("BFS 访问顺序: ");
    while (head < tail) {
        int v = queue[head++];
        printf("%d ", v);

        for (int w = 0; w < graph->vertex_count; w++) {
            if (graph->matrix[v][w] != 0 && !visited[w]) {
                visited[w] = true;
                distance[w] = distance[v] + 1;
                queue[tail++] = w;
            }
        }
    }
    printf("\n从 %d 出发的边数距离: ", start);
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("%d:%d ", i, distance[i]);
    }
    printf("\n");
}

static void dfs_visit(const Graph *graph, int v, bool *visited)
{
    visited[v] = true;
    printf("%d ", v);

    for (int w = 0; w < graph->vertex_count; w++) {
        if (graph->matrix[v][w] != 0 && !visited[w]) {
            dfs_visit(graph, w, visited);
        }
    }
}

static void dfs(const Graph *graph, int start)
{
    bool visited[MAX_VERTICES] = {false};
    printf("DFS 访问顺序: ");
    dfs_visit(graph, start, &visited[0]);
    printf("\n");
}

/* 对每个没访问过的点做一次 DFS，就统计出了连通分量 */
static int count_components(const Graph *graph)
{
    bool visited[MAX_VERTICES] = {false};
    int count = 0;

    for (int v = 0; v < graph->vertex_count; v++) {
        if (!visited[v]) {
            count++;
            printf("第 %d 个连通分量: ", count);
            dfs_visit(graph, v, &visited[0]);
            printf("\n");
        }
    }
    return count;
}

int main(void)
{
    /*
     * 6 个顶点，边：
     *   0-1, 0-2, 1-3, 2-3   （0、1、2、3 连成一片）
     *   4-5                  （4、5 单独一片）
     *
     *   0 --- 1
     *   |     |
     *   2 --- 3        4 --- 5
     */
    Graph graph;
    init_graph(&graph, 6);
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 2, 3);
    add_edge(&graph, 4, 5);

    print_matrix(&graph);
    printf("\n");

    bfs(&graph, 0);
    printf("\n");

    dfs(&graph, 0);
    printf("\n");

    printf("连通分量个数: %d\n", count_components(&graph));

    printf("\n存储方式怎么选:\n");
    printf("  邻接矩阵: O(V^2) 空间，查两点是否相连 O(1)，适合稠密图\n");
    printf("  邻接表:   O(V+E) 空间，适合稀疏图（大多数真实场景都是稀疏的）\n");
    return 0;
}
