#include "uf.h"

void uf_init(UF *u, int n)
{
    u->n = n;
    u->p = malloc((size_t)n * sizeof *u->p);
    u->rank = calloc((size_t)n, sizeof *u->rank);
    for (int i = 0; i < n; i++) {
        u->p[i] = i;
    }
}

void uf_free(UF *u)
{
    free(u->p);
    free(u->rank);
    memset(u, 0, sizeof *u);
}

int uf_find(UF *u, int x)
{
    return u->p[x] == x ? x : (u->p[x] = uf_find(u, u->p[x]));
}

bool uf_union(UF *u, int a, int b)
{
    a = uf_find(u, a);
    b = uf_find(u, b);
    if (a == b) {
        return false;
    }
    if (u->rank[a] < u->rank[b]) {
        int t = a;
        a = b;
        b = t;
    }
    u->p[b] = a;
    if (u->rank[a] == u->rank[b]) {
        u->rank[a]++;
    }
    return true;
}

bool uf_same(UF *u, int a, int b)
{
    return uf_find(u, a) == uf_find(u, b);
}
