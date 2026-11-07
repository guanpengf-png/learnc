/*
 * 章节：递归，猴子吃桃
 * 知识点：
 *   第 10 天还没吃的时候，只剩 1 个。这是结束条件。
 *   每一天吃掉“一半再加 1 个”，所以前一天的数量满足：
 *     今天 = 昨天 / 2 - 1
 *     昨天 = (今天 + 1) * 2
 *   函数参数用“第几天”，往第 10 天推，所以递归调用的是 day + 1，不是 day - 1。
 */
#include <stdio.h>

static int peach(int day)
{
    if (day == 10) {
        return 1;
    }
    return (peach(day + 1) + 1) * 2;
}

int main(void)
{
    printf("第 10 天: %d\n", peach(10));
    printf("第 9 天: %d\n", peach(9));
    printf("第 1 天: %d\n", peach(1));
    return 0;
}
