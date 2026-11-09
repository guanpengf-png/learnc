/* 第 45 章：Dijkstra / 简易 A*（堆做 open） */
#ifndef DS_PATH_H
#define DS_PATH_H

#include "../common.h"

typedef int (*TileCostFn)(int x, int y, void *ctx);
typedef bool (*PassableFn)(int x, int y, void *ctx);

/* prev[y][x] = 前驱编码 y*MAP_W+x，起点为 -1；不可达 -2 */
bool dijkstra_to(Point start, Point goal, PassableFn pass, TileCostFn cost, void *ctx,
                 int prev[MAP_H][MAP_W], int *out_dist);

/* 从 prev 反推路径，写入 out，返回长度；失败 0 */
int path_rebuild(Point start, Point goal, const int prev[MAP_H][MAP_W], Point *out, int maxn);

/* BFS 视野/移动范围格子数 */
int bfs_count(Point start, PassableFn pass, void *ctx, int max_steps);

#endif
