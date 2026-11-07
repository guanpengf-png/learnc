/*
 * 章节：字典树（Trie / 前缀树）
 * 知识点：
 *   字典树把字符串一个字符一层地挂到树上。根表示空串，每条边代表一个字符，
 *   从根走到某个结点，路径上的字符拼起来就是它的一个前缀。
 *   公共前缀只存一份：app、apple、apply 共用 a -> p -> p 这条路。
 *   每个结点存两样东西：
 *     children[26]：26 个小写字母分别指向下一个结点，没有就是 NULL
 *     is_end：这里是不是某个单词的结尾
 *   插入、查找、查前缀都是 O(m)，m 是字符串长度，和存了多少个单词无关。
 *   “查前缀”和“查单词”的区别：
 *     starts_with("app") 为真，只说明有单词以 app 开头；
 *     search("app") 为真，还要求 app 本身是某个单词的结尾（is_end）。
 *   代价：每个结点都要放 26 个指针，很占内存。单词少或字符集大时，
 *         可以把 children 换成哈希表，或用压缩字典树（把单链合并）。
 *   应用：输入法自动补全、拼写检查、单词游戏、IP 路由表、敏感词过滤。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET];
    bool is_end;
} TrieNode;

static TrieNode *create_node(void)
{
    /* calloc 会把 children 全部置为 NULL，is_end 置为 false */
    return calloc(1, sizeof(TrieNode));
}

static void trie_insert(TrieNode *root, const char *word)
{
    TrieNode *node = root;

    for (const char *p = word; *p != '\0'; p++) {
        int index = *p - 'a';
        if (index < 0 || index >= ALPHABET) {
            continue;      /* 这个例子只处理小写字母 */
        }
        if (node->children[index] == NULL) {
            node->children[index] = create_node();
            if (node->children[index] == NULL) {
                return;    /* 内存不足 */
            }
        }
        node = node->children[index];
    }
    node->is_end = true;
}

static bool trie_search(const TrieNode *root, const char *word)
{
    const TrieNode *node = root;

    for (const char *p = word; *p != '\0'; p++) {
        int index = *p - 'a';
        if (index < 0 || index >= ALPHABET || node->children[index] == NULL) {
            return false;
        }
        node = node->children[index];
    }
    return node->is_end;
}

static bool trie_starts_with(const TrieNode *root, const char *prefix)
{
    const TrieNode *node = root;

    for (const char *p = prefix; *p != '\0'; p++) {
        int index = *p - 'a';
        if (index < 0 || index >= ALPHABET || node->children[index] == NULL) {
            return false;
        }
        node = node->children[index];
    }
    return true;
}

/* 数一数以某个前缀开头的单词有多少个 */
static int count_words_under(const TrieNode *node)
{
    if (node == NULL) {
        return 0;
    }

    int count = node->is_end ? 1 : 0;
    for (int i = 0; i < ALPHABET; i++) {
        count += count_words_under(node->children[i]);
    }
    return count;
}

static int count_words_with_prefix(const TrieNode *root, const char *prefix)
{
    const TrieNode *node = root;

    for (const char *p = prefix; *p != '\0'; p++) {
        int index = *p - 'a';
        if (index < 0 || index >= ALPHABET || node->children[index] == NULL) {
            return 0;
        }
        node = node->children[index];
    }
    return count_words_under(node);
}

/* 从 node 往下走，把遇到的所有单词打印出来 */
static void collect_words(const TrieNode *node, char *buffer, int depth)
{
    if (node == NULL) {
        return;
    }
    if (node->is_end) {
        buffer[depth] = '\0';
        printf("  %s\n", buffer);
    }
    for (int i = 0; i < ALPHABET; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = (char)('a' + i);
            collect_words(node->children[i], buffer, depth + 1);
        }
    }
}

static void autocomplete(const TrieNode *root, const char *prefix)
{
    const TrieNode *node = root;

    for (const char *p = prefix; *p != '\0'; p++) {
        int index = *p - 'a';
        if (index < 0 || index >= ALPHABET || node->children[index] == NULL) {
            printf("  没有以 %s 开头的单词\n", prefix);
            return;
        }
        node = node->children[index];
    }

    char buffer[64];
    int len = (int)strlen(prefix);
    if (len >= (int)sizeof buffer) {
        return;
    }
    memcpy(buffer, prefix, (size_t)len);
    collect_words(node, buffer, len);
}

static void destroy_trie(TrieNode *node)
{
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET; i++) {
        destroy_trie(node->children[i]);
    }
    free(node);
}

int main(void)
{
    TrieNode *root = create_node();
    if (root == NULL) {
        printf("内存不足\n");
        return 1;
    }

    const char *words[] = {"apple", "app", "apply", "banana", "band", "bee"};
    int word_count = (int)(sizeof words / sizeof words[0]);

    for (int i = 0; i < word_count; i++) {
        trie_insert(root, words[i]);
    }

    printf("插入单词: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n\n");

    printf("查找完整单词:\n");
    printf("  app    -> %s\n", trie_search(root, "app") ? "存在" : "不存在");
    printf("  appl   -> %s\n", trie_search(root, "appl") ? "存在" : "不存在");
    printf("  banana -> %s\n", trie_search(root, "banana") ? "存在" : "不存在");

    printf("\n查前缀（不要求是完整单词）:\n");
    printf("  以 app 开头 -> %s\n", trie_starts_with(root, "app") ? "有" : "没有");
    printf("  以 ban  开头 -> %s\n", trie_starts_with(root, "ban") ? "有" : "没有");
    printf("  以 cat  开头 -> %s\n", trie_starts_with(root, "cat") ? "有" : "没有");

    printf("\n自动补全 app（%d 个候选）:\n", count_words_with_prefix(root, "app"));
    autocomplete(root, "app");

    printf("\n自动补全 ba:\n");
    autocomplete(root, "ba");

    destroy_trie(root);
    printf("\n释放完成\n");
    return 0;
}
