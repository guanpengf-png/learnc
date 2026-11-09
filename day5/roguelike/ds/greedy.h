/* 第 51 章：贪心找零反例 / 拾取建议 */
#ifndef DS_GREEDY_H
#define DS_GREEDY_H

#include "../common.h"

int greedy_change(int amount, const int *coins, int ncoin, int *used_out);
int optimal_change_3_4_1(int amount); /* 对 {4,3,1} 用 DP 最优枚数 */

#endif
