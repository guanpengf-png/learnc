/* 第 43 章：二分 / lower_bound / 线性查找 */
#ifndef DS_SEARCH_H
#define DS_SEARCH_H

#include "../common.h"
#include "sort.h"

size_t lower_bound_price(const BagRow *a, size_t n, int price);
int linear_find_id(const BagRow *a, size_t n, int id);

#endif
