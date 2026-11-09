#include "hash.h"

void hashmap_init(HashMap *m, size_t nb)
{
    m->nb = nb ? nb : 17;
    m->buckets = calloc(m->nb, sizeof *m->buckets);
}

void hashmap_free(HashMap *m)
{
    if (!m->buckets) {
        return;
    }
    for (size_t i = 0; i < m->nb; i++) {
        HashNode *n = m->buckets[i];
        while (n) {
            HashNode *nx = n->next;
            free(n);
            n = nx;
        }
    }
    free(m->buckets);
    memset(m, 0, sizeof *m);
}

static size_t hkey(const HashMap *m, int key)
{
    return (size_t)(unsigned)key % m->nb;
}

void hashmap_put(HashMap *m, int key, int value)
{
    size_t b = hkey(m, key);
    for (HashNode *n = m->buckets[b]; n; n = n->next) {
        if (n->key == key) {
            n->value = value;
            return;
        }
    }
    HashNode *n = malloc(sizeof *n);
    if (!n) {
        return;
    }
    n->key = key;
    n->value = value;
    n->next = m->buckets[b];
    m->buckets[b] = n;
}

bool hashmap_get(const HashMap *m, int key, int *out)
{
    for (HashNode *n = m->buckets[hkey(m, key)]; n; n = n->next) {
        if (n->key == key) {
            if (out) {
                *out = n->value;
            }
            return true;
        }
    }
    return false;
}

void hashmap_del(HashMap *m, int key)
{
    size_t b = hkey(m, key);
    HashNode **pp = &m->buckets[b];
    while (*pp) {
        if ((*pp)->key == key) {
            HashNode *dead = *pp;
            *pp = dead->next;
            free(dead);
            return;
        }
        pp = &(*pp)->next;
    }
}
