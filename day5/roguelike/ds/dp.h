/* 第 47 章：0-1 背包 / LIS / 编辑距离 */
#ifndef DS_DP_H
#define DS_DP_H

#include "../common.h"

int knapsack01(const int *w, const int *v, int n, int cap);
int lis_length(const int *a, int n);
int edit_distance(const char *a, const char *b);

#endif
