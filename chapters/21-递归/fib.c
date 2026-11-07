/*
 * 章节：递归求斐波那契数
 * 知识点：
 *   第 1 个和第 2 个是 1。从第 3 个开始，每一项等于前两项之和。
 *   结束条件是 n == 1 或 n == 2。
 *   这种写法会重复计算同一项，n 很大时又慢又容易把调用栈用完。这里只算第 10 项。
 *   第 10 项是 55。
 */
#include <stdio.h>

static int fib(int n)
{
    if (n == 1 || n == 2) {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

int main(void)
{
    int n = 10;
    printf("%d 的斐波那契数: %d\n", n, fib(n));
    return 0;
}
