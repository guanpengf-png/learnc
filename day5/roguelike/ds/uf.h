/* 第 52 章：并查集 */
#ifndef DS_UF_H
#define DS_UF_H

#include "../common.h"

typedef struct {
    int *p;
    int *rank;
    int n;
} UF;

void uf_init(UF *u, int n);
void uf_free(UF *u);
int uf_find(UF *u, int x);
bool uf_union(UF *u, int a, int b); /* true=合并成功；false=已同组（成环） */
bool uf_same(UF *u, int a, int b);

#endif
