/*
 * 练习：输出 a~z 和 Z~A
 * 知识点：
 *   char 本质是整数，所以可以写在 for 的循环变量里，也可以 ++。
 *   'a' 到 'z'、'A' 到 'Z' 在 ASCII 里是连续的。
 *   倒序就从 'Z' 开始，每次 --，直到 'A'。
 *   %c 按字符输出。
 */
#include <stdio.h>

int main(void)
{
    for (char letter = 'a'; letter <= 'z'; letter++) {
        printf("%c ", letter);
    }
    printf("\n");

    for (char letter = 'Z'; letter >= 'A'; letter--) {
        printf("%c ", letter);
    }
    printf("\n");
    return 0;
}
