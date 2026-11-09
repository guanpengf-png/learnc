#include "greedy.h"

int greedy_change(int amount, const int *coins, int ncoin, int *used_out)
{
    int left = amount;
    int used = 0;
    for (int i = 0; i < ncoin; i++) {
        while (left >= coins[i]) {
            left -= coins[i];
            used++;
        }
    }
    if (used_out) {
        *used_out = used;
    }
    return left == 0;
}

int optimal_change_3_4_1(int amount)
{
    int dp[64];
    for (int i = 0; i <= amount; i++) {
        dp[i] = INF_COST;
    }
    dp[0] = 0;
    int coins[] = {1, 3, 4};
    for (int a = 1; a <= amount; a++) {
        for (int c = 0; c < 3; c++) {
            if (a >= coins[c] && dp[a - coins[c]] + 1 < dp[a]) {
                dp[a] = dp[a - coins[c]] + 1;
            }
        }
    }
    return dp[amount];
}
