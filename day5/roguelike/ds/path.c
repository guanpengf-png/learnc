#include "path.h"
#include "heap.h"
#include "queue.h"

bool dijkstra_to(Point start, Point goal, PassableFn pass, TileCostFn cost, void *ctx,
                 int prev[MAP_H][MAP_W], int *out_dist)
{
    int dist[MAP_H][MAP_W];
    bool done[MAP_H][MAP_W];
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            dist[y][x] = INF_COST;
            prev[y][x] = -2;
            done[y][x] = false;
        }
    }
    MinHeap h;
    heap_init(&h);
    dist[start.y][start.x] = 0;
    prev[start.y][start.x] = -1;
    heap_push(&h, start.y * MAP_W + start.x, 0);

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    while (!heap_empty(&h)) {
        HeapItem z;
        heap_pop(&h, &z);
        int x = z.id % MAP_W;
        int y = z.id / MAP_W;
        if (done[y][x]) {
            continue;
        }
        done[y][x] = true;
        if (x == goal.x && y == goal.y) {
            break;
        }
        for (int k = 0; k < 4; k++) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (!pass(nx, ny, ctx)) {
                continue;
            }
            int step = cost(nx, ny, ctx);
            int nd = dist[y][x] + step;
            if (nd < dist[ny][nx]) {
                dist[ny][nx] = nd;
                prev[ny][nx] = y * MAP_W + x;
                /* A* 味道：priority = g + 曼哈顿启发 */
                int f = nd + absi(goal.x - nx) + absi(goal.y - ny);
                heap_push(&h, ny * MAP_W + nx, f);
            }
        }
    }
    heap_free(&h);
    if (out_dist) {
        *out_dist = dist[goal.y][goal.x];
    }
    return dist[goal.y][goal.x] < INF_COST;
}

int path_rebuild(Point start, Point goal, const int prev[MAP_H][MAP_W], Point *out, int maxn)
{
    if (prev[goal.y][goal.x] == -2) {
        return 0;
    }
    Point tmp[MAP_W * MAP_H];
    int n = 0;
    Point cur = goal;
    while (!(cur.x == start.x && cur.y == start.y)) {
        if (n >= MAP_W * MAP_H) {
            return 0;
        }
        tmp[n++] = cur;
        int id = prev[cur.y][cur.x];
        if (id < 0) {
            break;
        }
        cur = (Point){id % MAP_W, id / MAP_W};
    }
    int m = 0;
    for (int i = n - 1; i >= 0 && m < maxn; i--) {
        out[m++] = tmp[i];
    }
    return m;
}

int bfs_count(Point start, PassableFn pass, void *ctx, int max_steps)
{
    IntQueue q;
    queue_init(&q, MAP_W * MAP_H);
    bool seen[MAP_H][MAP_W] = {0};
    int steps[MAP_H][MAP_W];
    memset(steps, 0, sizeof steps);
    queue_push(&q, start.y * MAP_W + start.x);
    seen[start.y][start.x] = true;
    int count = 0;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    while (!queue_empty(&q)) {
        int id;
        queue_pop(&q, &id);
        int x = id % MAP_W;
        int y = id / MAP_W;
        count++;
        if (steps[y][x] >= max_steps) {
            continue;
        }
        for (int k = 0; k < 4; k++) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (!pass(nx, ny, ctx) || seen[ny][nx]) {
                continue;
            }
            seen[ny][nx] = true;
            steps[ny][nx] = steps[y][x] + 1;
            queue_push(&q, ny * MAP_W + nx);
        }
    }
    queue_free(&q);
    return count;
}
