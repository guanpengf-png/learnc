/*
 * 章节：用 continue 做「敲 7」
 * 知识点：
 *   跳过三类数：7 的倍数、个位是 7、十位是 7。
 *   i % 7 == 0   倍数
 *   i % 10 == 7  个位
 *   i / 10 == 7  十位。这里只考虑到 100，所以十位用整除就够了。
 *   三个条件是「或者」的关系，用 ||。
 */
#include <stdio.h>

int main(void)
{
    for (int i = 0; i <= 100; i++) {
        if (i % 7 == 0 || i % 10 == 7 || i / 10 == 7) {
            continue;
        }
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}
