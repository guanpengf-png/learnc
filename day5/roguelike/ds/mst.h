/* 第 46 章：Kruskal 最小生成树（按真实距离排序） */
#ifndef DS_MST_H
#define DS_MST_H

#include "../common.h"
#include "graph.h"

typedef struct {
    int a, b, w;
} MstEdge;

/* 输入房间中心，输出走廊边写入 out_edges，返回边数；并填充 graph */
int kruskal_rooms(const Point *rooms, int n, MstEdge *out_edges, int max_edges, Graph *g, int extra_cycles);

#endif
