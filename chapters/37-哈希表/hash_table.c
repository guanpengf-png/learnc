/*
 * 章节：哈希表（链地址法）
 * 知识点：
 *   哈希表用“关键字 -> 地址”的映射来存数据，理想情况下查找、插入、删除都是 O(1)。
 *   哈希函数把 key 算成一个数组下标。不同的 key 可能算出同一个下标，这叫哈希冲突。
 *   冲突无法完全避免，只能处理。链地址法：每个桶挂一条链表，算到同一个桶的元素都挂在链上。
 *   装填因子 = 元素个数 / 桶数。装填因子越大，链表越长，查找越慢。
 *   真实实现会在装填因子超过 0.75 左右时扩容（桶数翻倍）并重新散列，这里为了简单用固定桶数。
 *   平均 O(1) 的前提是“哈希均匀”；最坏情况所有 key 挤在一个桶里，退化成 O(n)。
 *   负数 key 直接取余会得到负数下标，所以先转成 unsigned 再取余。
 *   删除时用二级指针可以省掉“前驱结点”的分支判断。
 *   应用：字典、缓存（LRU 底层）、去重、编译器符号表、数据库哈希索引。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUCKET_COUNT 11

typedef struct Entry {
    int key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[BUCKET_COUNT];
    size_t size;
} HashTable;

static void init_table(HashTable *table)
{
    for (size_t i = 0; i < BUCKET_COUNT; i++) {
        table->buckets[i] = NULL;
    }
    table->size = 0;
}

/* 先转 unsigned 再取余，负数 key 也不会得到负下标 */
static size_t hash_key(int key)
{
    return (size_t)((unsigned int)key % BUCKET_COUNT);
}

/* 返回 true 表示新增，false 表示 key 已存在（更新）或内存不足 */
static bool put(HashTable *table, int key, int value)
{
    size_t index = hash_key(key);

    for (Entry *entry = table->buckets[index]; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            entry->value = value;      /* key 已存在，覆盖旧值 */
            return false;
        }
    }

    Entry *entry = malloc(sizeof *entry);
    if (entry == NULL) {
        return false;
    }
    entry->key = key;
    entry->value = value;
    entry->next = table->buckets[index];   /* 头插法，O(1) */
    table->buckets[index] = entry;
    table->size++;
    return true;
}

static bool get(const HashTable *table, int key, int *value)
{
    size_t index = hash_key(key);

    for (const Entry *entry = table->buckets[index]; entry != NULL; entry = entry->next) {
        if (entry->key == key) {
            *value = entry->value;
            return true;
        }
    }
    return false;
}

static bool remove_key(HashTable *table, int key)
{
    size_t index = hash_key(key);

    /* 二级指针 link 指向“指向当前结点的那个指针”，
       这样删除头结点和删除中间结点可以用同一套代码 */
    Entry **link = &table->buckets[index];

    while (*link != NULL) {
        Entry *entry = *link;
        if (entry->key == key) {
            *link = entry->next;
            free(entry);
            table->size--;
            return true;
        }
        link = &entry->next;
    }
    return false;
}

static void print_table(const HashTable *table)
{
    printf("哈希表（%d 个桶，%zu 个元素）:\n", BUCKET_COUNT, table->size);
    for (size_t i = 0; i < BUCKET_COUNT; i++) {
        printf("  桶 %2zu: ", i);
        for (const Entry *entry = table->buckets[i]; entry != NULL; entry = entry->next) {
            printf("[%d->%d] ", entry->key, entry->value);
        }
        printf("\n");
    }
}

static void destroy_table(HashTable *table)
{
    for (size_t i = 0; i < BUCKET_COUNT; i++) {
        Entry *entry = table->buckets[i];
        while (entry != NULL) {
            Entry *next = entry->next;
            free(entry);
            entry = next;
        }
        table->buckets[i] = NULL;
    }
    table->size = 0;
}

int main(void)
{
    HashTable table;
    init_table(&table);

    /* 1、12、23 对 11 取余都是 1，故意让它们撞在同一个桶 */
    int keys[] = {1, 12, 23, 7, 18, 29, 5, 16, 40, 51};
    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; i++) {
        put(&table, keys[i], (int)(i + 1) * 100);
    }

    printf("插入 10 个 key（注意 1/12/23 落在同一个桶）:\n");
    print_table(&table);

    int value = 0;
    if (get(&table, 23, &value)) {
        printf("查找 23 -> %d\n", value);
    }
    if (!get(&table, 100, &value)) {
        printf("查找 100 -> 不存在\n");
    }

    put(&table, 12, 999);
    get(&table, 12, &value);
    printf("更新 12 -> %d（元素个数仍然是 %zu）\n", value, table.size);

    remove_key(&table, 7);
    printf("删除 7 之后，元素个数 %zu\n", table.size);
    print_table(&table);

    destroy_table(&table);
    printf("释放后元素个数 %zu\n", table.size);
    return 0;
}
