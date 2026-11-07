/*
 * 章节：原码、反码、补码
 * 知识点：
 *   机器数：一个数在计算机里的二进制，最高位是符号位，0 正 1 负。
 *   真值：去掉符号位之后，这个机器数真正表示的数。
 *   正数的原码、反码、补码相同。
 *   负数：原码 = 符号位 1 + 绝对值；反码 = 符号位不变、其余取反；补码 = 反码 + 1。
 *   计算机存整数、做加减，用的是补码。这样减法可以变成加法，而且正零和负零是同一个编码。
 *
 * 下面用 8 位演示。int8_t 的位模式就是它的补码。
 * 案例：10 - 12 = 10 + (-12)。
 *   10   的补码 0000 1010
 *   -12  的原码 1000 1100，反码 1111 0011，补码 1111 0100
 *   相加               1111 1110，这是 -2 的补码
 */
#include <stdio.h>
#include <stdint.h>

static void print_bits(uint8_t value)
{
    for (int bit = 7; bit >= 0; bit--) {
        unsigned int mask = 1u << (unsigned int)bit;
        putchar((value & mask) != 0u ? '1' : '0');
        if (bit == 4) {
            putchar(' ');
        }
    }
}

int main(void)
{
    int8_t positive = 1;
    int8_t negative = -1;
    printf("+1 补码: ");
    print_bits((uint8_t)positive);
    printf("\n-1 补码: ");
    print_bits((uint8_t)negative);
    printf("\n");

    int8_t ten = 10;
    int8_t minus_twelve = -12;
    int8_t result = (int8_t)(ten + minus_twelve);
    printf("10      ");
    print_bits((uint8_t)ten);
    printf("\n-12     ");
    print_bits((uint8_t)minus_twelve);
    printf("\n10-12   ");
    print_bits((uint8_t)result);
    printf("  = %d\n", (int)result);
    return 0;
}
