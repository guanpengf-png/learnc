/* 第 35 章：栈 —— 悔棋快照、菜单返回、DFS 回退 */
#ifndef DS_STACK_H
#define DS_STACK_H

#include "../common.h"

typedef struct {
    int *data;
    size_t top, cap;
} IntStack;

void stack_init(IntStack *s, size_t cap);
void stack_free(IntStack *s);
bool stack_push(IntStack *s, int v);
bool stack_pop(IntStack *s, int *out);
bool stack_empty(const IntStack *s);

#endif
