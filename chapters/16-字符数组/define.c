/*
 * 章节：字符数组的定义
 * 知识点：
 *   存放字符的数组可以当成字符串来用。用 %s 输出时，遇到 '\0' 就停止。
 *   '\0' 是 ASCII 里的第 0 个字符，叫空字符。它不显示，只用来标记字符串结束。
 *   所以能当字符串用的字符数组，有效字符后面必须有一个 '\0'，这个位置也算一个元素。
 *   花括号赋值时：
 *     写了 '\0'：按你写的来。
 *     指定的长度比列出的字符多：多出来的位置自动是 '\0'，可以当字符串。
 *     长度刚好等于列出的字符个数，又没写 '\0'：末尾没有结束符，不能用 %s 去打印。
 *   讲义里 chs3 打出 jacktom，是因为 %s 越界读到了旁边的数组。这是未定义行为，不能当成规则。
 */
#include <stdio.h>

int main(void)
{
    char chs1[5] = {'a', 'b', 'c', 'd', '\0'};
    char chs2[4] = {'t', 'o', 'm'};
    char jack[] = {'j', 'a', 'c', 'k', '\0'};

    printf("%s\n", chs1);
    printf("%s\n", chs2);
    printf("%s\n", jack);

    /* 下面这种没有结束符，只能按字符一个个读，不能交给 %s：
     * char broken[] = {'j', 'a', 'c', 'k'};
     */
    char broken[] = {'j', 'a', 'c', 'k'};
    printf("没有结束符时，按字符输出: ");
    for (int i = 0; i < (int)(sizeof broken / sizeof broken[0]); i++) {
        printf("%c", broken[i]);
    }
    printf("\n");
    return 0;
}
