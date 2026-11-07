/*
 * 章节：string.h 里的字符串函数
 * 知识点：
 *   strlen(s) 统计 '\0' 之前的字符数，不算结束符。
 *   strcpy(目的, 来源) 把来源整段复制到目的，目的里原来的内容被盖掉。目的必须放得下，包括最后的 '\0'。
 *   strcat(目的, 来源) 把来源接到目的现有的 '\0' 位置。目的剩下的空间也必须够。
 *   讲义把 "hello" 放进刚好 6 字节的数组，再 strcat 一长串，会写越界。这里把目的数组开到 32 字节。
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char chs1[32] = "hello";
    char chs2[10] = "abcdefg";

    printf("chs1 的长度: %zu\n", strlen(chs1));
    printf("chs2 的长度: %zu\n", strlen(chs2));

    strcpy(chs1, "world");
    printf("复制后: %s\n", chs1);

    strcat(chs1, chs2);
    printf("拼接后: %s\n", chs1);
    return 0;
}
