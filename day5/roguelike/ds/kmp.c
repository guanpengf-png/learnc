#include "kmp.h"

static void build_pi(const char *p, int *pi, int m)
{
    int k = 0;
    pi[0] = 0;
    for (int i = 1; i < m; i++) {
        while (k && p[i] != p[k]) {
            k = pi[k - 1];
        }
        if (p[i] == p[k]) {
            k++;
        }
        pi[i] = k;
    }
}

bool kmp_contains(const char *text, const char *pat)
{
    int m = (int)strlen(pat);
    if (!m) {
        return true;
    }
    int pi[128];
    if (m >= 128) {
        return strstr(text, pat) != NULL;
    }
    build_pi(pat, pi, m);
    int k = 0;
    for (int i = 0; text[i]; i++) {
        while (k && text[i] != pat[k]) {
            k = pi[k - 1];
        }
        if (text[i] == pat[k]) {
            k++;
        }
        if (k == m) {
            return true;
        }
    }
    return false;
}

void kmp_filter_badwords(char *text)
{
    static const char *bad[] = {"idiot", "stupid", "hack", "cheat"};
    for (size_t b = 0; b < sizeof bad / sizeof *bad; b++) {
        int m = (int)strlen(bad[b]);
        int pi[64];
        build_pi(bad[b], pi, m);
        for (int i = 0, k = 0; text[i];) {
            while (k && text[i] != bad[b][k]) {
                k = pi[k - 1];
            }
            if (tolower((unsigned char)text[i]) == bad[b][k] || text[i] == bad[b][k]) {
                k++;
            }
            i++;
            if (k == m) {
                for (int t = i - m; t < i; t++) {
                    text[t] = '*';
                }
                k = pi[k - 1];
            }
        }
    }
}
