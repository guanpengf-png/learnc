/* 第 44 章：图（房间邻接表） */
#ifndef DS_GRAPH_H
#define DS_GRAPH_H

#include "../common.h"
#include "vector.h"

typedef struct {
    IntVec adj[MAX_ROOMS];
    int n;
} Graph;

void graph_init(Graph *g, int n);
void graph_free(Graph *g);
void graph_add_undirected(Graph *g, int a, int b);
bool graph_connected_dfs(const Graph *g);

#endif
