/*
 * 章节：用格式占位符按不同进制输出
 * 知识点：
 *   %d   十进制
 *   %x   十六进制，字母小写，不带前缀
 *   %#x  带 0x 前缀
 *   %#X  带 0X 前缀，字母大写
 *   100 的十六进制是 0x64，因为 6*16+4 = 100。
 */
#include <stdio.h>

int main(void)
{
    int num = 100;
    printf("十进制形式:%d\n", num);
    printf("十六进制形式:%x\n", num);
    printf("十六进制的0x小写形式:%#x\n", num);
    printf("十六进制的0X大写形式:%#X\n", num);
    return 0;
}
