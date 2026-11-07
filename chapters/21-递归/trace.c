/*
 * 章节：递归的执行顺序
 * 知识点：
 *   函数在自己的函数体里调用自己，就是递归。
 *   必须有一个能停下来的条件，而且每次调用都要更靠近这个条件，否则就是死递归。
 *   先递归再打印，会在返回的路上从最深的那层开始打印，所以是升序。
 *   先打印再递归，打印发生在进入下一层之前，所以是降序。
 *   讲义 show_num 里写了 printf("%d", n)，但参数名是 a，n 并不存在。下面按参数 a 修正。
 */
#include <stdio.h>

static void show_ascending(int a)
{
    if (a != 0) {
        show_ascending(a - 1);
    }
    printf("%d ", a);
}

static void count_down(int i)
{
    printf("%d ", i);
    if (i == 0) {
        return;
    }
    count_down(i - 1);
}

static void count_up(int i)
{
    printf("%d ", i);
    if (i == 5) {
        return;
    }
    count_up(i + 1);
}

static void trace(int n)
{
    printf("进入 %d\n", n);
    if (n > 1) {
        trace(n - 1);
    }
    printf("离开 %d\n", n);
}

int main(void)
{
    show_ascending(3);
    printf("\n");
    count_down(3);
    printf("\n");
    count_up(0);
    printf("\n");
    trace(3);
    return 0;
}
