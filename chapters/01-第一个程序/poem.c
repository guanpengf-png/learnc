/*
 * 章节：用 printf 输出内容
 * 知识点：
 *   printf 把双引号里的文本送到终端。
 *   \n 是换行转义字符，不是字母 n。
 *   没有 \n 时，下一次输出会接在同一行后面。
 */
#include <stdio.h>

int main(void)
{
    printf("吟诗一首\n");
    printf("锄禾日当午,\n");
    printf("地雷埋下土.\n");
    printf("春哥走过去,\n");
    printf("炸成二百五.\n");
    return 0;
}
