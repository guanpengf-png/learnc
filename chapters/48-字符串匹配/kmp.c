/*
 * 章节：字符串匹配（KMP 算法）
 * 知识点：
 *   问题：在长文本 text 里找模式串 pattern 出现的位置。
 *   朴素做法：从每个位置开始逐字比较，失配就从下一个位置重新来。
 *     最坏 O(n * m)，比如 text = "aaaaaaaab"，pattern = "aaab"，每次都要比很多次。
 *   KMP 的关键观察：已经比较过的部分里有信息，失配后不必从头再来。
 *   KMP 为模式串预处理一个 pi 数组（也叫 next 数组）：
 *     pi[i] 表示 pattern[0..i] 这一段中，最长的“真前缀 = 真后缀”的长度。
 *     例如 pattern = "ABABCABAB"：
 *       "ABAB" 的前缀 AB 和后缀 AB 相同，长度 2，所以 pi[3] = 2。
 *   匹配时用两个指针 i（文本）和 k（模式）：
 *     字符相同就一起前进；
 *     不同就令 k = pi[k-1]，把模式串往右滑到“还能对上”的位置，而不是退回 0。
 *   预处理 O(m)，匹配 O(n)，总共 O(n + m)。
 *   失配指针的思想也用于：AC 自动机（多模式匹配）、后缀数组、字符串压缩。
 *   应用：编辑器查找、DNA 序列比对、日志关键字搜索、入侵检测、敏感词过滤。
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_N 256

/* pi[i] = pattern[0..i] 的最长“真前缀 == 真后缀”的长度 */
static void build_pi(const char *pattern, int m, int *pi)
{
    pi[0] = 0;
    int k = 0;      /* 当前已经匹配上的前缀长度 */

    for (int i = 1; i < m; i++) {
        while (k > 0 && pattern[i] != pattern[k]) {
            k = pi[k - 1];      /* 回退到上一个可能的位置 */
        }
        if (pattern[i] == pattern[k]) {
            k++;
        }
        pi[i] = k;
    }
}

/*
 * 返回第一次出现的位置（找不到返回 -1），并通过 count 传出出现次数。
 * 统计时用 k = pi[k-1] 继续，可以正确处理重叠匹配，比如 "aaa" 里找 "aa"。
 */
static int kmp_search(const char *text, const char *pattern, const int *pi, int *count)
{
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);
    int k = 0;
    int first = -1;
    int found = 0;

    for (int i = 0; i < n; i++) {
        while (k > 0 && text[i] != pattern[k]) {
            k = pi[k - 1];
        }
        if (text[i] == pattern[k]) {
            k++;
        }

        if (k == m) {
            int position = i - m + 1;
            if (first == -1) {
                first = position;
            }
            found++;

            /* 继续找下一个（包括重叠的） */
            k = pi[k - 1];
        }
    }

    *count = found;
    return first;
}

/* 朴素匹配，用来对比 */
static int naive_search(const char *text, const char *pattern, int *count)
{
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);
    int first = -1;
    int found = 0;

    for (int i = 0; i + m <= n; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) {
            if (first == -1) {
                first = i;
            }
            found++;
        }
    }

    *count = found;
    return first;
}

int main(void)
{
    const char *text = "ABABDABACDABABCABAB";
    const char *pattern = "ABABCABAB";
    int m = (int)strlen(pattern);
    int pi[MAX_N] = {0};

    build_pi(pattern, m, pi);

    printf("文本:   %s\n", text);
    printf("模式:   %s\n", pattern);
    printf("pi 数组: ");
    for (int i = 0; i < m; i++) {
        printf("%d ", pi[i]);
    }
    printf("\n\n");

    int count = 0;
    int position = naive_search(text, pattern, &count);
    printf("朴素匹配: 第一次出现在 %d，共 %d 次\n", position, count);

    position = kmp_search(text, pattern, pi, &count);
    printf("KMP 匹配: 第一次出现在 %d，共 %d 次\n\n", position, count);

    /* 重叠匹配的例子 */
    const char *t2 = "aaaaa";
    const char *p2 = "aa";
    int pi2[8] = {0};
    build_pi(p2, 2, pi2);
    int count2 = 0;
    int pos2 = kmp_search(t2, p2, pi2, &count2);
    printf("在 \"%s\" 里找 \"%s\": 第一次 %d，共 %d 次（包含重叠）\n",
           t2, p2, pos2, count2);

    printf("\n为什么快: 失配时利用已经匹配的信息，模式串不会退回开头重扫。\n");
    return 0;
}
