/* 第 33 章：动态数组（怪物池 / 背包；空槽复用） */
#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "../common.h"

typedef struct {
    int *data;
    bool *alive; /* 空槽复用：alive[i]==false 表示可覆写 */
    size_t len;  /* 逻辑上活跃个数 */
    size_t cap;  /* 物理容量 */
    size_t next_free; /* 提示下一个可扫描起点 */
} IntPool;

void pool_init(IntPool *p, size_t cap);
void pool_free(IntPool *p);
int pool_alloc(IntPool *p, int value); /* 返回下标，失败 -1 */
void pool_release(IntPool *p, size_t i);
int pool_get(const IntPool *p, size_t i);

typedef struct {
    int *data;
    size_t len, cap;
} IntVec;

void ivec_init(IntVec *v);
void ivec_free(IntVec *v);
bool ivec_push(IntVec *v, int x);
bool ivec_remove_at(IntVec *v, size_t i);

#endif
