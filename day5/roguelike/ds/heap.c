#include "heap.h"

void heap_init(MinHeap *h)
{
    h->data = NULL;
    h->len = h->cap = 0;
}

void heap_free(MinHeap *h)
{
    free(h->data);
    memset(h, 0, sizeof *h);
}

static void swap(HeapItem *a, HeapItem *b)
{
    HeapItem t = *a;
    *a = *b;
    *b = t;
}

void heap_push(MinHeap *h, int id, int priority)
{
    if (h->len == h->cap) {
        size_t ncap = h->cap ? h->cap * 2 : 8;
        HeapItem *p = realloc(h->data, ncap * sizeof *p);
        if (!p) {
            return;
        }
        h->data = p;
        h->cap = ncap;
    }
    size_t i = h->len++;
    h->data[i] = (HeapItem){id, priority};
    while (i) {
        size_t p = (i - 1) / 2;
        if (h->data[p].priority <= h->data[i].priority) {
            break;
        }
        swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

bool heap_pop(MinHeap *h, HeapItem *out)
{
    if (!h->len) {
        return false;
    }
    if (out) {
        *out = h->data[0];
    }
    h->data[0] = h->data[--h->len];
    size_t i = 0;
    for (;;) {
        size_t l = i * 2 + 1, r = l + 1, m = i;
        if (l < h->len && h->data[l].priority < h->data[m].priority) {
            m = l;
        }
        if (r < h->len && h->data[r].priority < h->data[m].priority) {
            m = r;
        }
        if (m == i) {
            break;
        }
        swap(&h->data[i], &h->data[m]);
        i = m;
    }
    return true;
}

bool heap_empty(const MinHeap *h)
{
    return h->len == 0;
}
