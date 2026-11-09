#include "backtrack.h"

bool puzzle_check_perm(const int *target, const int *guess, int n)
{
    for (int i = 0; i < n; i++) {
        if (target[i] != guess[i]) {
            return false;
        }
    }
    return true;
}

static bool used_ok(const int *out, int pos, int val)
{
    for (int i = 0; i < pos; i++) {
        if (out[i] == val) {
            return false;
        }
    }
    return true;
}

static bool dfs(int n, int pos, int *out)
{
    if (pos == n) {
        return true;
    }
    for (int v = 1; v <= n; v++) {
        if (v == pos + 1) {
            continue; /* 错位约束 */
        }
        if (!used_ok(out, pos, v)) {
            continue;
        }
        out[pos] = v;
        if (dfs(n, pos + 1, out)) {
            return true;
        }
    }
    return false;
}

bool puzzle_derangement(int n, int *out)
{
    return dfs(n, 0, out);
}
