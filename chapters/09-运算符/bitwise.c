/*
 * 章节：位运算符
 * 知识点：
 *   位运算看的是补码，一位一位处理。
 *   &  两边都是 1 才是 1
 *   |  有一个 1 就是 1
 *   ^  相同为 0，不同为 1
 *   ~  按位取反，包括符号位。~n 的结果是 -n-1
 *   << 左移 n 位，相当于乘 2 的 n 次方（没有溢出时）
 *   >> 右移 n 位，无符号数相当于除 2 的 n 次方。
 *   对负数做 << 是未定义行为，严格编译会直接报错，所以不能写 -12 << 2。
 *   下面改用无符号补码演示：-12 左移 2 位得到 -48。
 *   负数右移也没有唯一规定。算术右移会在左边补 1，-12 >> 3 得到 -2，
 *   不是像整数除法那样把 -1.5 截成 -1。
 */
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    int a = 5; /* 0000 0101 */
    int b = 3; /* 0000 0011 */
    printf("a & b = %d\n", a & b); /* 0000 0001 = 1 */
    printf("a | b = %d\n", a | b); /* 0000 0111 = 7 */
    printf("a ^ b = %d\n", a ^ b); /* 0000 0110 = 6 */

    int c = 17;
    int d = -12;
    printf("~c = %d, c = %d\n", ~c, c); /* ~17 = -18 */
    printf("~d = %d, d = %d\n", ~d, d); /* ~(-12) = 11 */

    printf("17 << 2 = %d\n", 17 << 2); /* 68 */
    printf("17 >> 3 = %d\n", 17 >> 3); /* 2 */

    uint32_t neg_twelve = ~12u + 1u;
    uint32_t shifted_left = neg_twelve << 2;
    printf("-12 << 2 = %d\n", (int)(int32_t)shifted_left); /* -48 */

    uint32_t shifted_right = neg_twelve >> 3;
    shifted_right |= 0xFFFFFFFFu << (32u - 3u); /* 符号位是 1，左边补 1 */
    printf("-12 >> 3 = %d\n", (int)(int32_t)shifted_right);
    return 0;
}
