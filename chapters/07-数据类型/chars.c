/*
 * 章节：字符类型
 * 知识点：
 *   char 存一个字符，字面量用单引号。多个字符是字符串，不是 char。
 *   char 的本质是整数，存的是 ASCII 码。'A' 是 65，'a' 是 97。
 *   %c 按字符输出，%d 按整数输出同一个值。
 *   \t 是制表符，属于转义字符。
 *   signed char 范围 -128~127。200 超出范围，补码解释后是 -56：
 *     200 = 1100 1000，最高位是 1，表示负数，真值是 -56。
 *   unsigned char 范围 0~255，200 可以正常保存。
 *   没写 signed / unsigned 时，char 是否带符号由编译器决定。本机按有符号处理。
 */
#include <stdio.h>

int main(void)
{
    char c1 = 'A';
    char c2 = '9';
    char c3 = '\t';
    printf("c1 = %c, c2 = %c, c3 = %c(制表符)\n", c1, c2, c3);

    char c4 = 'A';
    char c5 = 97;
    printf("c4 = %c, c4 = %d\n", c4, c4);
    printf("c5 = %c, c5 = %d\n", c5, c5);
    printf("c4 + c5 = %d\n", c4 + c5); /* 65 + 97 = 162，发生整数提升 */

    signed char c6 = (signed char)200;
    unsigned char c7 = 200;
    char c8 = (char)200;
    printf("c6 = %d, c7 = %d, c8 = %d\n", c6, c7, c8);
    return 0;
}
