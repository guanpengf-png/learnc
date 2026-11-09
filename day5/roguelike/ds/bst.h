/* 第 39 章：BST 商店按价格 */
#ifndef DS_BST_H
#define DS_BST_H

#include "../common.h"

typedef struct ShopItem {
    int id;
    int price;
    const char *name;
} ShopItem;

typedef struct BstNode {
    ShopItem item;
    struct BstNode *l, *r;
} BstNode;

BstNode *bst_insert(BstNode *r, ShopItem item);
void bst_inorder(BstNode *r, void (*fn)(const ShopItem *, void *), void *ctx);
const ShopItem *bst_floor_price(BstNode *r, int budget); /* 不超过预算的最贵 */
void bst_free(BstNode *r);

#endif
