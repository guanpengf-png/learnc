#include "bst.h"

BstNode *bst_insert(BstNode *r, ShopItem item)
{
    if (!r) {
        BstNode *n = calloc(1, sizeof *n);
        if (n) {
            n->item = item;
        }
        return n;
    }
    if (item.price < r->item.price) {
        r->l = bst_insert(r->l, item);
    } else {
        r->r = bst_insert(r->r, item);
    }
    return r;
}

void bst_inorder(BstNode *r, void (*fn)(const ShopItem *, void *), void *ctx)
{
    if (!r) {
        return;
    }
    bst_inorder(r->l, fn, ctx);
    fn(&r->item, ctx);
    bst_inorder(r->r, fn, ctx);
}

const ShopItem *bst_floor_price(BstNode *r, int budget)
{
    const ShopItem *best = NULL;
    while (r) {
        if (r->item.price <= budget) {
            best = &r->item;
            r = r->r;
        } else {
            r = r->l;
        }
    }
    return best;
}

void bst_free(BstNode *r)
{
    if (!r) {
        return;
    }
    bst_free(r->l);
    bst_free(r->r);
    free(r);
}
