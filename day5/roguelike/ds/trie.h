/* 第 49 章：Trie 命令/物品补全 */
#ifndef DS_TRIE_H
#define DS_TRIE_H

#include "../common.h"

typedef struct TrieNode {
    struct TrieNode *c[27]; /* a-z + '_' */
    bool end;
    char word[32];
} TrieNode;

typedef struct {
    TrieNode root;
} Trie;

void trie_init(Trie *t);
void trie_free(Trie *t);
void trie_add(Trie *t, const char *word);
int trie_complete(Trie *t, const char *prefix, char out[][32], int maxn);

#endif
