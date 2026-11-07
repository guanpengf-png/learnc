/*
 * 章节：整数类型
 * 知识点：
 *   整型用来存整数。默认有符号（signed），unsigned 表示无符号，不能表示负数。
 *   常用 int。范围不够再用 long long。
 *   字面量后缀：L / LL 表示 long / long long，U 表示无符号，可以组合，如 LU、LLU。
 *   占位符要和类型匹配，否则是未定义行为：
 *     %hd %hu  short / unsigned short
 *     %d  %u   int / unsigned int
 *     %ld %lu  long / unsigned long
 *     %lld %llu long long / unsigned long long
 *   各类型占几个字节和系统有关。64 位 macOS 上 long 是 8 字节，Windows 上常见是 4 字节。
 */
#include <stdio.h>

int main(void)
{
    short s1 = 10;
    signed short s2 = -10;
    unsigned short s3 = 20;
    printf("short: s1 = %hd, s2 = %hd, s3 = %hu\n", s1, s2, s3);

    int i1 = 10;
    signed int i2 = -10;
    unsigned int i3 = 20u;
    printf("int: i1 = %d, i2 = %d, i3 = %u\n", i1, i2, i3);

    long l1 = 100L;
    signed long l2 = -100L;
    unsigned long l3 = 1000LU;
    printf("long: l1 = %ld, l2 = %ld, l3 = %lu\n", l1, l2, l3);

    long long ll1 = 1000LL;
    signed long long ll2 = -1000LL;
    unsigned long long ll3 = 1000LLU;
    printf("long long: ll1 = %lld, ll2 = %lld, ll3 = %llu\n", ll1, ll2, ll3);
    return 0;
}
