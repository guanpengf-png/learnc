/*
 * 章节：回溯
 * 知识点：
 *   回溯 = 深度优先搜索 + 撤销选择。可以想成走迷宫：一条路走到黑，走不通就退回上一个路口换条路。
 *   把求解过程想成一棵“决策树”：
 *     每一步做一个选择，进入下一层；发现此路不通，就退回上一层换一个选择。
 *   通用模板：
 *     void backtrack(当前状态) {
 *         if (到达终点) { 记录答案; return; }
 *         for (每个候选选择) {
 *             做选择;                 // 修改状态
 *             backtrack(下一层);      // 递归
 *             撤销选择;               // 恢复状态，关键！
 *         }
 *     }
 *   “撤销”是回溯的灵魂。不撤销，后面的分支就会看到被污染的状态，结果全错。
 *   剪枝：提前判断某条路不可能有解，直接跳过，不再往下搜。
 *   回溯的时间复杂度通常是指数级（O(2^n) 或 O(n!)），因为最坏要枚举所有组合。
 *   和动态规划的区别：DP 让每个小问题只算一次；回溯是把所有可能的路径都试一遍。
 *   应用：八皇后、数独、走迷宫、全排列/组合/子集、正则表达式、图着色。
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* ---------------- 1. 全排列 ---------------- */
static int permutation_count = 0;

static void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void permute(int *a, int start, int n)
{
    if (start == n) {
        printf("  ");
        for (int i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
        permutation_count++;
        return;
    }

    for (int i = start; i < n; i++) {
        swap_int(&a[start], &a[i]);       /* 做选择 */
        permute(a, start + 1, n);         /* 进入下一层 */
        swap_int(&a[start], &a[i]);       /* 撤销选择 */
    }
}

/* ---------------- 2. 八皇后 ---------------- */
#define QUEEN_N 8

static int queen_count = 0;
static int first_solution[QUEEN_N];

/*
 * 逐行放皇后。row 表示当前要放第几行。
 * cols[c]          ：第 c 列有没有皇后
 * diag1[row + col] ：主对角线（左上到右下）有没有皇后
 * diag2[row - col + N - 1] ：副对角线（右上到左下），加偏移避免负下标
 */
static void solve_queens(int row, bool *cols, bool *diag1, bool *diag2, int *placement)
{
    if (row == QUEEN_N) {
        queen_count++;
        if (queen_count == 1) {
            memcpy(first_solution, placement, sizeof first_solution);
        }
        return;
    }

    for (int col = 0; col < QUEEN_N; col++) {
        int d1 = row + col;
        int d2 = row - col + QUEEN_N - 1;

        if (cols[col] || diag1[d1] || diag2[d2]) {
            continue;      /* 这个位置会被攻击，剪枝 */
        }

        /* 做选择 */
        cols[col] = true;
        diag1[d1] = true;
        diag2[d2] = true;
        placement[row] = col;

        solve_queens(row + 1, cols, diag1, diag2, placement);

        /* 撤销选择 */
        cols[col] = false;
        diag1[d1] = false;
        diag2[d2] = false;
    }
}

static void print_board(const int *placement, int n)
{
    for (int row = 0; row < n; row++) {
        printf("  ");
        for (int col = 0; col < n; col++) {
            printf(placement[row] == col ? "Q " : ". ");
        }
        printf("\n");
    }
}

int main(void)
{
    /* 1. 全排列 */
    int numbers[] = {1, 2, 3};
    printf("1 2 3 的全排列:\n");
    permute(numbers, 0, 3);
    printf("共 %d 种\n\n", permutation_count);

    /* 2. 八皇后 */
    bool cols[QUEEN_N] = {false};
    bool diag1[2 * QUEEN_N - 1] = {false};
    bool diag2[2 * QUEEN_N - 1] = {false};
    int placement[QUEEN_N] = {0};

    solve_queens(0, cols, diag1, diag2, placement);

    printf("八皇后共有 %d 种解\n", queen_count);
    printf("第一种解（Q 表示皇后）:\n");
    print_board(first_solution, QUEEN_N);

    printf("\n每一行放一个皇后，冲突就用剪枝跳过，走不通就退回上一行。\n");
    return 0;
}
