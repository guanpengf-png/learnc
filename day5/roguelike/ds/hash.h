/* 第 37 章：哈希表 —— 物品 ID、坐标空间哈希 */
#ifndef DS_HASH_H
#define DS_HASH_H

#include "../common.h"

typedef struct HashNode {
    int key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **buckets;
    size_t nb;
} HashMap;

void hashmap_init(HashMap *m, size_t nb);
void hashmap_free(HashMap *m);
void hashmap_put(HashMap *m, int key, int value);
bool hashmap_get(const HashMap *m, int key, int *out);
void hashmap_del(HashMap *m, int key);

static inline int cell_key(int x, int y)
{
    return y * MAP_W + x;
}

#endif
