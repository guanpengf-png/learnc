#include "avl.h"

static int h(AvlNode *n)
{
    return n ? n->height : 0;
}

static int mx(int a, int b)
{
    return a > b ? a : b;
}

static void upd(AvlNode *n)
{
    n->height = 1 + mx(h(n->l), h(n->r));
}

static int bal(AvlNode *n)
{
    return h(n->l) - h(n->r);
}

static AvlNode *rot_r(AvlNode *y)
{
    AvlNode *x = y->l;
    AvlNode *t = x->r;
    x->r = y;
    y->l = t;
    upd(y);
    upd(x);
    return x;
}

static AvlNode *rot_l(AvlNode *x)
{
    AvlNode *y = x->r;
    AvlNode *t = y->l;
    y->l = x;
    x->r = t;
    upd(x);
    upd(y);
    return y;
}

static int cmp_score(Score a, Score b)
{
    if (a.total != b.total) {
        return (a.total > b.total) - (a.total < b.total);
    }
    if (a.kills != b.kills) {
        return (a.kills > b.kills) - (a.kills < b.kills);
    }
    if (a.gold != b.gold) {
        return (a.gold > b.gold) - (a.gold < b.gold);
    }
    return (a.depth > b.depth) - (a.depth < b.depth);
}

AvlNode *avl_insert(AvlNode *n, Score s)
{
    if (!n) {
        AvlNode *x = calloc(1, sizeof *x);
        if (x) {
            x->s = s;
            x->height = 1;
        }
        return x;
    }
    if (cmp_score(s, n->s) < 0) {
        n->l = avl_insert(n->l, s);
    } else {
        n->r = avl_insert(n->r, s);
    }
    upd(n);
    int b = bal(n);
    if (b > 1 && cmp_score(s, n->l->s) < 0) {
        return rot_r(n);
    }
    if (b < -1 && cmp_score(s, n->r->s) >= 0) {
        return rot_l(n);
    }
    if (b > 1) {
        n->l = rot_l(n->l);
        return rot_r(n);
    }
    if (b < -1) {
        n->r = rot_r(n->r);
        return rot_l(n);
    }
    return n;
}

static AvlNode *min_node(AvlNode *n)
{
    while (n && n->l) {
        n = n->l;
    }
    return n;
}

AvlNode *avl_remove_min(AvlNode *n)
{
    if (!n) {
        return NULL;
    }
    if (!n->l) {
        AvlNode *r = n->r;
        free(n);
        return r;
    }
    n->l = avl_remove_min(n->l);
    upd(n);
    int b = bal(n);
    if (b < -1 && bal(n->r) > 0) {
        n->r = rot_r(n->r);
        return rot_l(n);
    }
    if (b < -1) {
        return rot_l(n);
    }
    if (b > 1 && bal(n->l) < 0) {
        n->l = rot_l(n->l);
        return rot_r(n);
    }
    if (b > 1) {
        return rot_r(n);
    }
    (void)min_node;
    return n;
}

void avl_each_desc(AvlNode *n, void (*fn)(const Score *, void *), void *ctx)
{
    if (!n) {
        return;
    }
    avl_each_desc(n->r, fn, ctx);
    fn(&n->s, ctx);
    avl_each_desc(n->l, fn, ctx);
}

void avl_free(AvlNode *n)
{
    if (!n) {
        return;
    }
    avl_free(n->l);
    avl_free(n->r);
    free(n);
}

int avl_count(AvlNode *n)
{
    return n ? 1 + avl_count(n->l) + avl_count(n->r) : 0;
}
