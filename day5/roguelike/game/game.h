#ifndef GAME_H
#define GAME_H

#include "../common.h"
#include "../ds/avl.h"
#include "../ds/bst.h"
#include "../ds/graph.h"
#include "../ds/hash.h"
#include "../ds/heap.h"
#include "../ds/list.h"
#include "../ds/queue.h"
#include "../ds/stack.h"
#include "../ds/trie.h"
#include "../ds/vector.h"

typedef enum {
    ENT_NONE = 0,
    ENT_PLAYER,
    ENT_MONSTER,
    ENT_ITEM
} EntKind;

typedef struct {
    EntKind kind;
    Point pos;
    int hp, max_hp;
    int atk;
    int speed; /* 越小越先动 */
    int energy;
    int faction; /* 并查集阵营 id */
    int item_id;
    bool alive;
} Entity;

typedef struct {
    int id;
    const char *name;
    int price, weight, heal, atk, rare, value;
} ItemDef;

typedef struct {
    int id;
    int count;
} BagSlot;

typedef struct {
    char tiles[MAP_H][MAP_W];
    Point rooms[MAX_ROOMS];
    int room_n;
    Graph room_graph;
} DungeonMap;

typedef struct {
    int px, py, gold, kills, depth, hp;
    int bag_n;
    BagSlot bag[32];
} UndoSnap;

typedef struct {
    DungeonMap map;
    Entity ents[MAX_ENTITIES];
    int ent_n;
    int player_id;
    IntPool monster_pool; /* 第 33：怪物槽 */
    HashMap item_by_id;   /* 第 37 */
    HashMap spatial;      /* 坐标 -> entity index */
    Buff *player_buffs;   /* 第 34 */
    IntQueue event_q;     /* 第 36 */
    MinHeap turn_heap;    /* 第 41 */
    Trie cmds;            /* 第 49 */
    AvlNode *board;       /* 第 40 */
    UndoSnap undos[MAX_UNDO];
    int undo_top;
    char logs[MAX_LOG][96];
    int log_n;
    int puzzle[4];
    bool puzzle_open;
    bool running;
    BagSlot bag[32];
    int bag_n;
    int gold, kills, depth;
} Game;

void game_init(Game *g);
void game_free(Game *g);
void game_loop(Game *g);

#endif
