/* 第 50 章：回溯 —— 机关门排列锁 */
#ifndef DS_BACKTRACK_H
#define DS_BACKTRACK_H

#include "../common.h"

bool puzzle_check_perm(const int *target, const int *guess, int n);

/* 生成 1..n 的一个错位排列（out[i] != i+1） */
bool puzzle_derangement(int n, int *out);

#endif
