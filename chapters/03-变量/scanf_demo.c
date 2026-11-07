/*
 * 章节：用 scanf 把键盘输入存进变量
 * 知识点：
 *   scanf 和 printf 都在 <stdio.h> 里。
 *   变量名前必须写 &，这是取地址：告诉 scanf 把数据写到哪块内存。
 *   格式串里的分隔符，就是用户输入时必须使用的分隔符。
 *     "%d %d %d"  三个数用空格（或换行）隔开
 *     "%d,%d"     两个数用逗号隔开，逗号两边不要加空格
 *
 * 试运行时依次输入：
 *   10
 *   20 30 40
 *   50,60
 */
#include <stdio.h>

int main(void)
{
    int a;
    printf("请您输入一个数字\n");
    if (scanf("%d", &a) != 1) {
        return 1;
    }
    printf("您刚刚输入的数据是:%d \n", a);

    int b, c, d;
    printf("请您输入三个数字,每个数字使用空格隔开\n");
    if (scanf("%d %d %d", &b, &c, &d) != 3) {
        return 1;
    }
    printf("您输入的三个数字分别是:%d,%d,%d \n", b, c, d);

    int e, f;
    printf("请输入两个数字,并使用'逗号'分隔 \n");
    if (scanf("%d,%d", &e, &f) != 2) {
        return 1;
    }
    printf("您输入的数字分别是:%d,%d \n", e, f);
    return 0;
}
