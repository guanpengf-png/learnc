#include "trie.h"

static int idx(char ch)
{
    if (ch == '_') {
        return 26;
    }
    if (ch >= 'a' && ch <= 'z') {
        return ch - 'a';
    }
    if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A';
    }
    return -1;
}

void trie_init(Trie *t)
{
    memset(t, 0, sizeof *t);
}

static void free_node(TrieNode *n)
{
    if (!n) {
        return;
    }
    for (int i = 0; i < 27; i++) {
        free_node(n->c[i]);
        free(n->c[i]);
    }
}

void trie_free(Trie *t)
{
    for (int i = 0; i < 27; i++) {
        free_node(t->root.c[i]);
        free(t->root.c[i]);
        t->root.c[i] = NULL;
    }
}

void trie_add(Trie *t, const char *word)
{
    TrieNode *n = &t->root;
    for (const char *p = word; *p; p++) {
        int i = idx(*p);
        if (i < 0) {
            return;
        }
        if (!n->c[i]) {
            n->c[i] = calloc(1, sizeof(TrieNode));
        }
        n = n->c[i];
    }
    n->end = true;
    snprintf(n->word, sizeof n->word, "%s", word);
}

static void collect(TrieNode *n, char out[][32], int *count, int maxn)
{
    if (!n || *count >= maxn) {
        return;
    }
    if (n->end) {
        snprintf(out[*count], 32, "%s", n->word);
        (*count)++;
    }
    for (int i = 0; i < 27; i++) {
        collect(n->c[i], out, count, maxn);
    }
}

int trie_complete(Trie *t, const char *prefix, char out[][32], int maxn)
{
    TrieNode *n = &t->root;
    for (const char *p = prefix; *p; p++) {
        int i = idx(*p);
        if (i < 0 || !n->c[i]) {
            return 0;
        }
        n = n->c[i];
    }
    int count = 0;
    collect(n, out, &count, maxn);
    return count;
}
