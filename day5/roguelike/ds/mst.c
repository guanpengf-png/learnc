#include "mst.h"
#include "uf.h"

static int cmp_edge(const void *a, const void *b)
{
    const MstEdge *x = a;
    const MstEdge *y = b;
    return (x->w > y->w) - (x->w < y->w);
}

int kruskal_rooms(const Point *rooms, int n, MstEdge *out_edges, int max_edges, Graph *g, int extra_cycles)
{
    MstEdge cand[MAX_ROOMS * MAX_ROOMS];
    int cn = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cand[cn++] = (MstEdge){i, j, dist2(rooms[i], rooms[j])};
        }
    }
    qsort(cand, (size_t)cn, sizeof *cand, cmp_edge);

    UF u;
    uf_init(&u, n);
    graph_init(g, n);
    int used = 0;
    for (int i = 0; i < cn && used < max_edges; i++) {
        if (uf_union(&u, cand[i].a, cand[i].b)) {
            out_edges[used++] = cand[i];
            graph_add_undirected(g, cand[i].a, cand[i].b);
        }
    }
    /* 再随机补几条成环，让地图有回路 */
    for (int i = 0; i < cn && extra_cycles > 0 && used < max_edges; i++) {
        if (!uf_same(&u, cand[i].a, cand[i].b)) {
            continue;
        }
        /* 已同组的边会成环；挑后面一些边补 */
        if (rand() % 3 == 0) {
            out_edges[used++] = cand[i];
            graph_add_undirected(g, cand[i].a, cand[i].b);
            extra_cycles--;
        }
    }
    uf_free(&u);
    return used;
}
