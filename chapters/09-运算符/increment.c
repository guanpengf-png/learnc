/*
 * 章节：自增和自减
 * 知识点：
 *   ++、-- 一定会修改操作数，这叫副作用。
 *   写在后面：表达式先用旧值，然后变量再变。
 *   写在前面：变量先变，表达式再用新值。
 *   不要在一个表达式里对同一个变量多次自增，结果没有保证。
 */
#include <stdio.h>

int main(void)
{
    {
        int x = 10;
        int y = x++; /* y 拿到 10，然后 x 变成 11 */
        printf("后加: x = %d, y = %d\n", x, y);
    }
    {
        int x = 10;
        int y = ++x; /* x 先变成 11，y 拿到 11 */
        printf("前加: x = %d, y = %d\n", x, y);
    }
    {
        int x = 10;
        int y = x--;
        printf("后减: x = %d, y = %d\n", x, y);
    }
    {
        int x = 10;
        int y = --x;
        printf("前减: x = %d, y = %d\n", x, y);
    }
    {
        int x = 10;
        int y = 3;
        int z = x-- + ++y; /* 用 x 的旧值 10，y 先变成 4，10 + 4 = 14，x 最后是 9 */
        printf("混合: x = %d, y = %d, z = %d\n", x, y, z);
    }
    return 0;
}
