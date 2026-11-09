#include "queue.h"

void queue_init(IntQueue *q, size_t cap)
{
    q->data = malloc(cap * sizeof *q->data);
    q->cap = q->data ? cap : 0;
    q->head = q->tail = q->count = 0;
}

void queue_free(IntQueue *q)
{
    free(q->data);
    memset(q, 0, sizeof *q);
}

bool queue_push(IntQueue *q, int v)
{
    if (q->count >= q->cap) {
        return false;
    }
    q->data[q->tail] = v;
    q->tail = (q->tail + 1) % q->cap;
    q->count++;
    return true;
}

bool queue_pop(IntQueue *q, int *out)
{
    if (!q->count) {
        return false;
    }
    if (out) {
        *out = q->data[q->head];
    }
    q->head = (q->head + 1) % q->cap;
    q->count--;
    return true;
}

bool queue_empty(const IntQueue *q)
{
    return q->count == 0;
}
