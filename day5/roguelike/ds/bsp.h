/* 第 38 章：BSP 二叉树切分地图 */
#ifndef DS_BSP_H
#define DS_BSP_H

#include "../common.h"

typedef struct BspNode {
    int x, y, w, h;
    struct BspNode *left, *right;
    Point room; /* 叶子房间中心 */
    bool leaf;
} BspNode;

BspNode *bsp_build(int x, int y, int w, int h, int depth);
void bsp_free(BspNode *n);
int bsp_collect_rooms(BspNode *n, Point *rooms, int maxn);

#endif
