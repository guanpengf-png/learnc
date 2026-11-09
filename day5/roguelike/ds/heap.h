/* 第 41 章：小根堆 —— 行动速度调度 / Dijkstra open / Top K */
#ifndef DS_HEAP_H
#define DS_HEAP_H

#include "../common.h"

typedef struct {
    int id;
    int priority; /* 越小越先 */
} HeapItem;

typedef struct {
    HeapItem *data;
    size_t len, cap;
} MinHeap;

void heap_init(MinHeap *h);
void heap_free(MinHeap *h);
void heap_push(MinHeap *h, int id, int priority);
bool heap_pop(MinHeap *h, HeapItem *out);
bool heap_empty(const MinHeap *h);

#endif
