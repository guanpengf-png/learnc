/* 第 40 章：AVL 排行榜（支持插入/删除/按名次遍历） */
#ifndef DS_AVL_H
#define DS_AVL_H

#include "../common.h"

typedef struct Score {
    int total;
    int gold, kills, depth;
    char name[16];
} Score;

typedef struct AvlNode {
    Score s;
    int height;
    struct AvlNode *l, *r;
} AvlNode;

AvlNode *avl_insert(AvlNode *n, Score s);
AvlNode *avl_remove_min(AvlNode *n); /* 删掉最低分，用于保榜容量 */
void avl_each_desc(AvlNode *n, void (*fn)(const Score *, void *), void *ctx);
void avl_free(AvlNode *n);
int avl_count(AvlNode *n);

#endif
