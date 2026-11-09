/* 第 42 章：qsort 与手写归并（稳定性对比） */
#ifndef DS_SORT_H
#define DS_SORT_H

#include "../common.h"

typedef struct {
    int id;
    int price;
    int rare;
    int weight;
    int order; /* 原始顺序，归并用它保持稳定 */
    const char *name;
} BagRow;

void sort_by_price_qsort(BagRow *a, size_t n);
void sort_by_price_mergesort(BagRow *a, size_t n); /* 稳定 */
void sort_by_rare(BagRow *a, size_t n);
void sort_by_weight(BagRow *a, size_t n);

#endif
