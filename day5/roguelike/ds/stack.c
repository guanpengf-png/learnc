#include "stack.h"

void stack_init(IntStack *s, size_t cap)
{
    s->data = malloc(cap * sizeof *s->data);
    s->top = 0;
    s->cap = s->data ? cap : 0;
}

void stack_free(IntStack *s)
{
    free(s->data);
    memset(s, 0, sizeof *s);
}

bool stack_push(IntStack *s, int v)
{
    if (s->top >= s->cap) {
        return false;
    }
    s->data[s->top++] = v;
    return true;
}

bool stack_pop(IntStack *s, int *out)
{
    if (!s->top) {
        return false;
    }
    if (out) {
        *out = s->data[--s->top];
    } else {
        s->top--;
    }
    return true;
}

bool stack_empty(const IntStack *s)
{
    return s->top == 0;
}
