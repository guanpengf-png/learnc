/*
 * 章节：sizeof
 * 知识点：
 *   sizeof 得到类型、变量或字面量占用的字节数。1 字节 = 8 位。
 *   返回类型是 size_t（无符号），占位符用 %zu。用 %d 是错误的。
 *   类型名必须加括号：sizeof(int)。变量和字面量的括号可以省略。
 *   字符常量 'a' 的类型是 int，不是 char，所以 sizeof 'a' 是 4。
 *   char 变量才是 1 字节。
 *   下面 long、long double 的结果以你这台机器为准。
 *   讲义按 Windows 写成 long = 4、long double = 16；64 位 macOS 上 long 通常是 8。
 */
#include <stdio.h>

int main(void)
{
    printf("char占字节大小为:%zu\n", sizeof(char));
    printf("short占字节大小为:%zu\n", sizeof(short));
    printf("int占字节大小为:%zu\n", sizeof(int));
    printf("long占字节大小为:%zu\n", sizeof(long));
    printf("long long占字节大小为:%zu\n", sizeof(long long));
    printf("float占字节大小为:%zu\n", sizeof(float));
    printf("double占字节大小为:%zu\n", sizeof(double));
    printf("long double占字节大小为:%zu\n", sizeof(long double));

    printf("字符a的字节大小:%zu\n", sizeof 'a');
    printf("整数100的字节大小:%zu\n", sizeof 100);
    printf("浮点数3.14f的字节大小:%zu\n", sizeof 3.14f);

    char a = 'a';
    int b = 10;
    long long c = 100;
    double d = 100.345;
    printf("变量a的字节大小为:%zu\n", sizeof a);
    printf("变量b的字节大小为:%zu\n", sizeof b);
    printf("变量c的字节大小为:%zu\n", sizeof c);
    printf("变量d的字节大小为:%zu\n", sizeof d);
    return 0;
}
