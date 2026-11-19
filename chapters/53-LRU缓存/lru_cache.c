/*
 * 章节：LRU 缓存
 * 知识点：
 *   LRU 是「最近最少使用」淘汰策略：容量满了，先淘汰最久没被访问的那个。
 *   要做到查找 O(1)、淘汰 O(1)，需要两样东西同时工作：
 *     1. 哈希表：key 直接映射到结点，解决「找得到」；
 *     2. 双向链表：结点按访问时间排队，头部最新、尾部最旧，解决「淘汰谁」。
 *   关键是让同一个结点同时挂在两条链上：prev/next 给双向链表用，hnext 给哈希桶拉链用。
 *   这样不需要第二份拷贝，也不会出现两份数据不一致。
 *
 *   链表用两个哨兵结点 head 和 tail，它们不存数据。
 *   有了哨兵，插入和删除都不用特判「空链表」或「头尾」。
 *
 *   哈希桶用「拉链法」：冲突时挂在同一条 hnext 链上。
 *   删除时用「指针的指针」遍历，可以省掉一个 hprev 指针：
 *     LruNode **pp = &buckets[i];
 *     while (*pp != node) pp = &(*pp)->hnext;
 *     *pp = node->hnext;
 *
 *   还有一个容易写错的顺序：容量满时，必须「先 malloc 新结点，成功后再淘汰旧结点」。
 *   如果先淘汰再 malloc，malloc 一失败，就白白丢了一条数据。
 *
 *   get 和 put 都是均摊 O(1)。桶数取 2 的幂，用位与代替取模更快。
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ---------- 数据结构 ---------- */

typedef struct LruNode {
    int key;
    int value;
    struct LruNode *prev;  /* 双向链表：前驱，朝 head 方向 */
    struct LruNode *next;  /* 双向链表：后继，朝 tail 方向 */
    struct LruNode *hnext; /* 哈希桶拉链：同一个桶里的下一个结点 */
} LruNode;

typedef struct {
    int capacity;       /* 容量，必须 > 0 */
    int size;           /* 当前结点数 */
    size_t bucket_count;/* 桶数量，取 2 的幂 */
    LruNode **buckets;  /* 桶数组，每个元素是一条拉链的头 */
    LruNode head;       /* 哨兵：head.next 是最新结点 */
    LruNode tail;       /* 哨兵：tail.prev 是最旧结点 */
} LruCache;

/* ---------- 内部工具：哈希 ---------- */

static size_t hash_bucket_index(const LruCache *cache, int key)
{
    /* Knuth 乘法散列：先把 key 打散，再取低位 */
    uint32_t mixed = (uint32_t)key * 2654435761u;
    return (size_t)mixed & (cache->bucket_count - 1);
}

static LruNode *hash_find(const LruCache *cache, int key)
{
    LruNode *node = cache->buckets[hash_bucket_index(cache, key)];
    while (node != NULL && node->key != key) {
        node = node->hnext;
    }
    return node;
}

static void hash_insert(LruCache *cache, LruNode *node)
{
    size_t index = hash_bucket_index(cache, node->key);
    node->hnext = cache->buckets[index];
    cache->buckets[index] = node;
}

/* 用指针的指针删除，不需要 hprev */
static void hash_remove(LruCache *cache, LruNode *node)
{
    size_t index = hash_bucket_index(cache, node->key);
    LruNode **link = &cache->buckets[index];
    while (*link != node) {
        assert(*link != NULL); /* 走到空还没找到，说明结点不在桶里，是 bug */
        link = &(*link)->hnext;
    }
    *link = node->hnext;
}

/* ---------- 内部工具：双向链表 ---------- */

static void list_insert_between(LruNode *node, LruNode *prev, LruNode *next)
{
    node->prev = prev;
    node->next = next;
    prev->next = node;
    next->prev = node;
}

static void list_remove(LruNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static void list_push_front(LruCache *cache, LruNode *node)
{
    list_insert_between(node, &cache->head, cache->head.next);
}

/* 访问过的结点要挪到最前，表示「刚刚用过」 */
static void list_move_to_front(LruCache *cache, LruNode *node)
{
    if (cache->head.next == node) {
        return; /* 已经在最前，不用动 */
    }
    list_remove(node);
    list_push_front(cache, node);
}

/* ---------- 对外接口 ---------- */

LruCache *lru_cache_create(int capacity)
{
    if (capacity <= 0) {
        return NULL;
    }

    LruCache *cache = malloc(sizeof *cache);
    if (cache == NULL) {
        return NULL;
    }

    /* 桶数留余量，让平均每条拉链不超过 0.5 个结点 */
    size_t bucket_count = 16;
    while (bucket_count < (size_t)capacity * 2) {
        bucket_count <<= 1;
    }

    cache->buckets = calloc(bucket_count, sizeof *cache->buckets);
    if (cache->buckets == NULL) {
        free(cache);
        return NULL;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->bucket_count = bucket_count;

    /* 把两个哨兵接成空链表：head <-> tail */
    cache->head.prev = NULL;
    cache->head.next = &cache->tail;
    cache->head.hnext = NULL;
    cache->tail.prev = &cache->head;
    cache->tail.next = NULL;
    cache->tail.hnext = NULL;

    return cache;
}

void lru_cache_destroy(LruCache *cache)
{
    if (cache == NULL) {
        return;
    }

    LruNode *node = cache->head.next;
    while (node != &cache->tail) {
        LruNode *next = node->next; /* 先记住 next，再 free */
        free(node);
        node = next;
    }

    free(cache->buckets);
    free(cache);
}

/* 命中返回 1，并把 value 写进 out_value；未命中返回 0 */
int lru_cache_get(LruCache *cache, int key, int *out_value)
{
    if (cache == NULL) {
        return 0;
    }

    LruNode *node = hash_find(cache, key);
    if (node == NULL) {
        return 0;
    }

    list_move_to_front(cache, node); /* 用过就算最新 */

    if (out_value != NULL) {
        *out_value = node->value;
    }
    return 1;
}

/* 成功返回 0，失败返回 -1 */
int lru_cache_put(LruCache *cache, int key, int value)
{
    if (cache == NULL) {
        return -1;
    }

    /* 已经存在：改值 + 提到最前 */
    LruNode *node = hash_find(cache, key);
    if (node != NULL) {
        node->value = value;
        list_move_to_front(cache, node);
        return 0;
    }

    /* 先申请新结点，成功了再淘汰，避免 malloc 失败时白白丢数据 */
    node = malloc(sizeof *node);
    if (node == NULL) {
        return -1;
    }
    node->key = key;
    node->value = value;
    node->hnext = NULL;

    if (cache->size == cache->capacity) {
        LruNode *victim = cache->tail.prev; /* 最旧的结点 */
        hash_remove(cache, victim);
        list_remove(victim);
        free(victim);
        cache->size--;
    }

    hash_insert(cache, node);
    list_push_front(cache, node);
    cache->size++;

    return 0;
}

/* ---------- 测试 ---------- */

static void check_get(LruCache *cache, int key, int expect_hit, int expect_value)
{
    int value = 0;
    int hit = lru_cache_get(cache, key, &value);

    assert(hit == expect_hit);
    if (hit) {
        assert(value == expect_value);
    }
}

static void test_basic(void)
{
    LruCache *cache = lru_cache_create(2);
    assert(cache != NULL);

    int rc = lru_cache_put(cache, 1, 10);
    assert(rc == 0);
    rc = lru_cache_put(cache, 2, 20);
    assert(rc == 0);

    check_get(cache, 1, 1, 10); /* 命中后，1 变成最新 */

    /* 容量 2 已满，插入 3 会淘汰最旧的 2 */
    rc = lru_cache_put(cache, 3, 30);
    assert(rc == 0);
    check_get(cache, 2, 0, 0);
    check_get(cache, 3, 1, 30);

    /* 更新已有的 key：值变了，并且被提到最前 */
    rc = lru_cache_put(cache, 1, 11);
    assert(rc == 0);
    check_get(cache, 1, 1, 11);

    lru_cache_destroy(cache);
}

static void test_capacity_one(void)
{
    LruCache *cache = lru_cache_create(1);
    assert(cache != NULL);

    int rc = lru_cache_put(cache, 7, 70);
    assert(rc == 0);
    rc = lru_cache_put(cache, 8, 80);
    assert(rc == 0);

    check_get(cache, 7, 0, 0); /* 7 被淘汰 */
    check_get(cache, 8, 1, 80);

    lru_cache_destroy(cache);
}

static void test_stress(void)
{
    LruCache *cache = lru_cache_create(64);
    assert(cache != NULL);

    for (int i = 0; i < 100000; i++) {
        int rc = lru_cache_put(cache, i, i * 2);
        assert(rc == 0);
    }

    check_get(cache, 99999, 1, 199998); /* 最后一个还在 */
    check_get(cache, 0, 0, 0);          /* 最早的一定被淘汰了 */

    lru_cache_destroy(cache);
}

static void test_invalid_argument(void)
{
    assert(lru_cache_create(0) == NULL);
    assert(lru_cache_create(-5) == NULL);
    assert(lru_cache_get(NULL, 1, NULL) == 0);
    assert(lru_cache_put(NULL, 1, 1) == -1);

    lru_cache_destroy(NULL); /* 传 NULL 也不能崩 */
}

static void demo(void)
{
    LruCache *cache = lru_cache_create(2);
    assert(cache != NULL);

    lru_cache_put(cache, 1, 10);
    lru_cache_put(cache, 2, 20);
    puts("写入 (1,10) (2,20)，容量 2");

    int value = 0;
    if (lru_cache_get(cache, 1, &value)) {
        printf("get(1) 命中，value = %d\n", value);
    }

    lru_cache_put(cache, 3, 30);
    puts("写入 (3,30)，应淘汰最久没用的 key = 2");

    printf("get(2) -> %s\n", lru_cache_get(cache, 2, &value) ? "命中" : "未命中");
    printf("get(3) -> %s\n", lru_cache_get(cache, 3, &value) ? "命中" : "未命中");

    lru_cache_destroy(cache);
}

int main(void)
{
    test_basic();
    test_capacity_one();
    test_stress();
    test_invalid_argument();

    demo();

    puts("全部测试通过");
    return 0;
}
