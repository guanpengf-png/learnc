#include "bsp.h"

static BspNode *make_leaf(int x, int y, int w, int h)
{
    BspNode *n = calloc(1, sizeof *n);
    if (!n) {
        return NULL;
    }
    n->x = x;
    n->y = y;
    n->w = w;
    n->h = h;
    n->leaf = true;
    n->room = (Point){x + w / 2, y + h / 2};
    return n;
}

BspNode *bsp_build(int x, int y, int w, int h, int depth)
{
    if (depth <= 0 || w < 10 || h < 8) {
        return make_leaf(x, y, w, h);
    }
    BspNode *n = calloc(1, sizeof *n);
    if (!n) {
        return NULL;
    }
    n->x = x;
    n->y = y;
    n->w = w;
    n->h = h;
    n->leaf = false;
    bool split_h = (w > h * 1.2) ? false : (h > w * 1.2) ? true : (rand() % 2 == 0);
    if (split_h) {
        int cut = h / 3 + rand() % (h / 3 + 1);
        n->left = bsp_build(x, y, w, cut, depth - 1);
        n->right = bsp_build(x, y + cut, w, h - cut, depth - 1);
    } else {
        int cut = w / 3 + rand() % (w / 3 + 1);
        n->left = bsp_build(x, y, cut, h, depth - 1);
        n->right = bsp_build(x + cut, y, w - cut, h, depth - 1);
    }
    return n;
}

void bsp_free(BspNode *n)
{
    if (!n) {
        return;
    }
    bsp_free(n->left);
    bsp_free(n->right);
    free(n);
}

int bsp_collect_rooms(BspNode *n, Point *rooms, int maxn)
{
    if (!n || maxn <= 0) {
        return 0;
    }
    if (n->leaf) {
        rooms[0] = n->room;
        return 1;
    }
    int a = bsp_collect_rooms(n->left, rooms, maxn);
    int b = bsp_collect_rooms(n->right, rooms + a, maxn - a);
    return a + b;
}
