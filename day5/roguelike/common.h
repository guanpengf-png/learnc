#ifndef ROGUELIKE_COMMON_H
#define ROGUELIKE_COMMON_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAP_W 48
#define MAP_H 20
#define MAX_ROOMS 12
#define MAX_ENTITIES 96
#define MAX_LOG 64
#define MAX_UNDO 16
#define BAG_CAP_WEIGHT 12
#define INF_COST 1000000

#define TILE_WALL '#'
#define TILE_FLOOR '.'
#define TILE_MUD '~'
#define TILE_TRAP '^'
#define TILE_DOOR '+'

typedef struct {
    int x, y;
} Point;

static inline int clampi(int v, int lo, int hi)
{
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

static inline int absi(int v)
{
    return v < 0 ? -v : v;
}

static inline int dist2(Point a, Point b)
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

#endif
