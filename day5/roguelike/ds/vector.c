#include "vector.h"

void pool_init(IntPool *p, size_t cap)
{
    p->data = calloc(cap, sizeof *p->data);
    p->alive = calloc(cap, sizeof *p->alive);
    p->len = 0;
    p->cap = cap;
    p->next_free = 0;
}

void pool_free(IntPool *p)
{
    free(p->data);
    free(p->alive);
    memset(p, 0, sizeof *p);
}

int pool_alloc(IntPool *p, int value)
{
    for (size_t k = 0; k < p->cap; k++) {
        size_t i = (p->next_free + k) % p->cap;
        if (!p->alive[i]) {
            p->alive[i] = true;
            p->data[i] = value;
            p->len++;
            p->next_free = (i + 1) % p->cap;
            return (int)i;
        }
    }
    size_t ncap = p->cap ? p->cap * 2 : 8;
    int *nd = realloc(p->data, ncap * sizeof *nd);
    bool *na = realloc(p->alive, ncap * sizeof *na);
    if (!nd || !na) {
        free(nd);
        free(na);
        return -1;
    }
    for (size_t i = p->cap; i < ncap; i++) {
        na[i] = false;
        nd[i] = 0;
    }
    p->data = nd;
    p->alive = na;
    size_t i = p->cap;
    p->cap = ncap;
    p->alive[i] = true;
    p->data[i] = value;
    p->len++;
    p->next_free = i + 1;
    return (int)i;
}

void pool_release(IntPool *p, size_t i)
{
    if (i >= p->cap || !p->alive[i]) {
        return;
    }
    p->alive[i] = false;
    p->len--;
    p->next_free = i;
}

int pool_get(const IntPool *p, size_t i)
{
    if (i >= p->cap || !p->alive[i]) {
        return 0;
    }
    return p->data[i];
}

void ivec_init(IntVec *v)
{
    v->data = NULL;
    v->len = v->cap = 0;
}

void ivec_free(IntVec *v)
{
    free(v->data);
    memset(v, 0, sizeof *v);
}

bool ivec_push(IntVec *v, int x)
{
    if (v->len == v->cap) {
        size_t ncap = v->cap ? v->cap * 2 : 4;
        int *p = realloc(v->data, ncap * sizeof *p);
        if (!p) {
            return false;
        }
        v->data = p;
        v->cap = ncap;
    }
    v->data[v->len++] = x;
    return true;
}

bool ivec_remove_at(IntVec *v, size_t i)
{
    if (i >= v->len) {
        return false;
    }
    memmove(&v->data[i], &v->data[i + 1], (v->len - i - 1) * sizeof *v->data);
    v->len--;
    return true;
}
