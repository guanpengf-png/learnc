/*
 * 章节：动态规划（DP）
 * 知识点：
 *   动态规划适合“能把大问题拆成小问题，而且小问题会被重复计算”的题。
 *   两个前提：
 *     1. 最优子结构：大问题的最优解，可以由小问题的最优解拼出来。
 *     2. 重叠子问题：同一个子问题会被算很多次（比如递归求斐波那契）。
 *   DP 的核心不是背代码，而是想清楚三件事：
 *     状态：dp[i] 或 dp[i][j] 到底表示什么？
 *     转移：dp[i] 从哪些更小的状态来？写出方程。
 *     边界：dp[0] 等初始值是什么？计算顺序要保证用到的值已经算好。
 *   写法有两种：
 *     自顶向下（记忆化搜索）：递归 + 数组缓存，写到哪算到哪，好想但可能递归很深。
 *     自底向上（递推）：从最小的问题开始填表，通常更快、不占调用栈。
 *   例子：
 *     斐波那契：dp[i] = dp[i-1] + dp[i-2]
 *     0-1 背包：dp[i][c] = max(不拿第 i 件, 拿第 i 件)
 *     最长上升子序列：dp[i] = 以 a[i] 结尾的最长长度
 *   应用：背包问题、编辑距离、股票买卖、路径计数、资源分配、最短路径、字符串匹配。
 */
#include <stdio.h>
#include <string.h>

#define MAX_N 50

/* ---------------- 1. 斐波那契：记忆化 vs 递推 ---------------- */

/* 自顶向下：memo[n] == -1 表示还没算过 */
static long long fib_memo(int n, long long *memo)
{
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];          /* 算过就直接用，避免重复计算 */
    }
    memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    return memo[n];
}

/* 自底向上：只保留前两个值，空间 O(1) */
static long long fib_iter(int n)
{
    if (n <= 1) {
        return n;
    }
    long long prev2 = 0;   /* fib(0) */
    long long prev1 = 1;   /* fib(1) */
    long long current = 0;

    for (int i = 2; i <= n; i++) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}

/* ---------------- 2. 0-1 背包 ---------------- */
/*
 * 有 n 件物品，第 i 件重量 weights[i]、价值 values[i]。
 * 背包容量 capacity，每件最多拿一次，求能装下的最大价值。
 *
 * 状态：dp[i][c] = 只看前 i 件物品、容量为 c 时的最大价值。
 * 转移：dp[i][c] = max(dp[i-1][c],                      不拿第 i 件
 *                     dp[i-1][c - w] + v)               拿第 i 件（前提是装得下）
 */
static int knapsack_2d(const int *weights, const int *values, int n, int capacity)
{
    int dp[10][20] = {0};      /* 这个例子 n <= 9，容量 <= 19 */

    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= capacity; c++) {
            dp[i][c] = dp[i - 1][c];    /* 先假设不拿 */

            if (weights[i - 1] <= c) {
                int take = dp[i - 1][c - weights[i - 1]] + values[i - 1];
                if (take > dp[i][c]) {
                    dp[i][c] = take;
                }
            }
        }
    }
    return dp[n][capacity];
}

/*
 * 一维滚动数组：dp[c] 只保留“上一行”的数据。
 * 容量必须从大到小遍历！
 * 因为 dp[c] 要用 dp[c - w]（更小的容量）的旧值，
 * 从大到小才不会让本行新算的值被重复使用（那就变成完全背包了）。
 */
static int knapsack_1d(const int *weights, const int *values, int n, int capacity)
{
    int dp[20] = {0};

    for (int i = 0; i < n; i++) {
        for (int c = capacity; c >= weights[i]; c--) {
            int take = dp[c - weights[i]] + values[i];
            if (take > dp[c]) {
                dp[c] = take;
            }
        }
    }
    return dp[capacity];
}

/* ---------------- 3. 最长上升子序列（LIS） ---------------- */
/*
 * 状态：dp[i] = 以 a[i] 结尾的最长上升子序列长度。
 * 转移：dp[i] = 1 + max(dp[j])，其中 j < i 且 a[j] < a[i]。
 * 答案：所有 dp[i] 里的最大值。
 * 时间复杂度 O(n^2)；用贪心 + 二分可以做到 O(n log n)。
 */
static int lis_length(const int *a, int n)
{
    int dp[MAX_N];
    int best = 0;

    for (int i = 0; i < n; i++) {
        dp[i] = 1;                  /* 至少包含自己 */
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
        if (dp[i] > best) {
            best = dp[i];
        }
    }
    return best;
}

int main(void)
{
    /* 1. 斐波那契 */
    long long memo[MAX_N];
    for (int i = 0; i < MAX_N; i++) {
        memo[i] = -1;
    }
    printf("fib(40) 记忆化 = %lld\n", fib_memo(40, memo));
    printf("fib(40) 递推   = %lld\n", fib_iter(40));
    printf("如果不缓存，fib(40) 要算大约 3 亿次加法\n\n");

    /* 2. 0-1 背包 */
    int weights[] = {2, 3, 4, 5};
    int values[] = {3, 4, 5, 6};
    int n = (int)(sizeof weights / sizeof weights[0]);
    int capacity = 8;

    printf("背包容量 %d，物品 (重量,价值): ", capacity);
    for (int i = 0; i < n; i++) {
        printf("(%d,%d) ", weights[i], values[i]);
    }
    printf("\n");

    printf("二维 DP 最大价值 = %d\n", knapsack_2d(weights, values, n, capacity));
    printf("一维 DP 最大价值 = %d（容量倒序遍历）\n\n",
           knapsack_1d(weights, values, n, capacity));

    /* 3. 最长上升子序列 */
    int nums[] = {10, 9, 2, 5, 3, 7, 101, 18};
    int count = (int)(sizeof nums / sizeof nums[0]);

    printf("数组: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n最长上升子序列长度 = %d\n", lis_length(nums, count));
    printf("例如 2, 3, 7, 101 或 2, 3, 7, 18，长度都是 4\n");

    return 0;
}
