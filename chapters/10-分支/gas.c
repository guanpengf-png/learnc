/*
 * 章节：嵌套分支
 * 知识点：
 *   分支里面还可以再写分支。外层先决定大类，内层再决定细节。
 *   层数建议不超过 3 层。
 *   92 号：不足 40 升是 4 元/升，达到 40 升是 3.5 元/升。
 *   95 号：不足 40 升是 5 元/升，达到 40 升是 4.5 元/升。
 *   金额用 double。%.2f 保留两位小数，适合表示元和角分。
 */
#include <stdio.h>

static void quote(int oil_number, int liters)
{
    double price = 0.0;
    switch (oil_number) {
    case 92:
        price = 4.0;
        if (liters >= 40) {
            price = 3.5;
        }
        break;
    case 95:
        price = 5.0;
        if (liters >= 40) {
            price = 4.5;
        }
        break;
    default:
        printf("没有 %d 号油\n", oil_number);
        return;
    }
    printf("%d 号油 %d 升，需要 %.2f 元\n", oil_number, liters, price * liters);
}

int main(void)
{
    quote(92, 30);
    quote(92, 40);
    quote(95, 30);
    quote(95, 40);
    quote(98, 10);
    return 0;
}
