/*
 * 章节：赋值运算符
 * 知识点：
 *   = 不是比较，它把右边的值写进左边的变量，表达式的值就是写入之后的值。
 *   左边必须是变量。+=、-=、*=、/=、%= 是先运算再写回。
 *   b += a 等价于 b = b + a，但左边的 b 只求值一次。
 *   移位和按位赋值在补码上进行：<<= >>= &= |= ^=。
 *   不要把赋值表达式直接塞进 printf 的参数列表。
 *   参数的计算顺序没有规定，和旁边的 a、b 混在一起会读到不确定的值。
 */
#include <stdio.h>

int main(void)
{
    {
        int a = 10;
        int b = 5;
        int value = (b = a);
        printf("b = a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 10;
        int b = 5;
        int value = (b += a);
        printf("b += a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 10;
        int b = 5;
        int value = (b -= a);
        printf("b -= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 10;
        int b = 3;
        int value = (b *= a);
        printf("b *= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 5;
        int b = 10;
        int value = (b /= a);
        printf("b /= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 3;
        int b = 10;
        int value = (b %= a);
        printf("b %%= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 3;
        int b = 2;
        int value = (b <<= a); /* 2 << 3 = 16 */
        printf("b <<= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 3;
        int b = 16;
        int value = (b >>= a); /* 16 >> 3 = 2 */
        printf("b >>= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 3;
        int b = 2; /* 0011 & 0010 = 0010 */
        int value = (b &= a);
        printf("b &= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 3;
        int b = 2; /* 0011 | 0010 = 0011 */
        int value = (b |= a);
        printf("b |= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    {
        int a = 3;
        int b = 2; /* 0011 ^ 0010 = 0001 */
        int value = (b ^= a);
        printf("b ^= a 的值:%d, a = %d, b = %d\n", value, a, b);
    }
    return 0;
}
