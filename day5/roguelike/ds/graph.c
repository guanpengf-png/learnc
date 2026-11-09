#include "graph.h"
#include "stack.h"

void graph_init(Graph *g, int n)
{
    g->n = n;
    for (int i = 0; i < MAX_ROOMS; i++) {
        ivec_init(&g->adj[i]);
    }
}

void graph_free(Graph *g)
{
    for (int i = 0; i < MAX_ROOMS; i++) {
        ivec_free(&g->adj[i]);
    }
    g->n = 0;
}

void graph_add_undirected(Graph *g, int a, int b)
{
    ivec_push(&g->adj[a], b);
    ivec_push(&g->adj[b], a);
}

bool graph_connected_dfs(const Graph *g)
{
    if (g->n <= 0) {
        return true;
    }
    bool seen[MAX_ROOMS] = {0};
    IntStack st;
    stack_init(&st, (size_t)g->n + 4);
    stack_push(&st, 0);
    seen[0] = true;
    int found = 0;
    while (!stack_empty(&st)) {
        int v;
        stack_pop(&st, &v);
        found++;
        for (size_t i = 0; i < g->adj[v].len; i++) {
            int w = g->adj[v].data[i];
            if (!seen[w]) {
                seen[w] = true;
                stack_push(&st, w);
            }
        }
    }
    stack_free(&st);
    return found == g->n;
}
