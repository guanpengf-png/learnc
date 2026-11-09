#include "dp.h"

int knapsack01(const int *w, const int *v, int n, int cap)
{
    int *dp = calloc((size_t)cap + 1, sizeof *dp);
    if (!dp) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        for (int c = cap; c >= w[i]; c--) {
            if (dp[c - w[i]] + v[i] > dp[c]) {
                dp[c] = dp[c - w[i]] + v[i];
            }
        }
    }
    int ans = dp[cap];
    free(dp);
    return ans;
}

int lis_length(const int *a, int n)
{
    int *d = malloc((size_t)n * sizeof *d);
    int best = 0;
    for (int i = 0; i < n; i++) {
        d[i] = 1;
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && d[j] + 1 > d[i]) {
                d[i] = d[j] + 1;
            }
        }
        if (d[i] > best) {
            best = d[i];
        }
    }
    free(d);
    return best;
}

int edit_distance(const char *a, const char *b)
{
    int n = (int)strlen(a);
    int m = (int)strlen(b);
    int dp[64][64];
    if (n >= 64 || m >= 64) {
        return -1;
    }
    for (int i = 0; i <= n; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        dp[0][j] = j;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int c = a[i - 1] != b[j - 1];
            int x = dp[i - 1][j] + 1;
            int y = dp[i][j - 1] + 1;
            int z = dp[i - 1][j - 1] + c;
            dp[i][j] = x < y ? (x < z ? x : z) : (y < z ? y : z);
        }
    }
    return dp[n][m];
}
