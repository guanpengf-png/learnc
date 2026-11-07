/*
 * 章节：字符数组的长度和遍历
 * 知识点：
 *   sizeof 字符数组 = 有效字符数 + 1 个 '\0'。
 *   strlen 只数字符，遇到 '\0' 就停，不算结束符。它声明在 <string.h>。
 *   讲义把 "love" 的字节数注释成 4，输出却是 5。sizeof 的 5 才对，4 是 strlen 的结果。
 *   遍历可见字符时，循环到 '\0' 之前。把 '\0' 用 %c 打出来看起来像一个空位。
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char chs[] = "love";

    printf("sizeof = %zu\n", sizeof chs);
    printf("strlen = %zu\n", strlen(chs));
    printf("%s\n", chs);
    printf("下标 2 的字符: %c\n", chs[2]);

    for (int i = 0; chs[i] != '\0'; i++) {
        printf("%c ", chs[i]);
    }
    printf("\n");
    return 0;
}
