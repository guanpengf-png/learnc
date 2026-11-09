/* 第 48 章：KMP 日志搜索 / 敏感词 */
#ifndef DS_KMP_H
#define DS_KMP_H

#include "../common.h"

bool kmp_contains(const char *text, const char *pat);
void kmp_filter_badwords(char *text); /* 命中敏感词则替换为 * */

#endif
