/* 第 36 章：队列 —— 行动/事件、BFS */
#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "../common.h"

typedef struct {
    int *data;
    size_t cap, head, tail, count;
} IntQueue;

void queue_init(IntQueue *q, size_t cap);
void queue_free(IntQueue *q);
bool queue_push(IntQueue *q, int v);
bool queue_pop(IntQueue *q, int *out);
bool queue_empty(const IntQueue *q);

#endif
