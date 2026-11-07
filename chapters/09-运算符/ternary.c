/*
 * 章节：三元运算符
 * 知识点：
 *   条件 ? 表达式1 : 表达式2
 *   条件非 0 时，整个表达式的值是表达式 1，否则是表达式 2。
 *   只有被选中的那一边会计算。a > b 为真时，b++ 不会执行。
 *   优先级整体偏低。复杂表达式用括号控制顺序，不要靠记忆优先级。
 *   大致顺序：一元 > 算术 > 关系 > 逻辑 > 三元 > 赋值。
 */
#include <stdio.h>

int main(void)
{
    int a = 10;
    int b = 5;
    printf("较大值 = %d, a = %d, b = %d\n", a > b ? a : b, a, b);

    int c = a > b ? a++ : b++;
    printf("c = %d, a = %d, b = %d\n", c, a, b); /* c 拿到自增前的 10，a 变成 11 */

    int x = 10;
    int y = 3;
    int z = 20;
    int max = x > y ? (x > z ? x : z) : (y > z ? y : z);
    printf("三个数字中的最大值 = %d\n", max);
    return 0;
}
