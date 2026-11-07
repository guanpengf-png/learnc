/*
 * 章节：函数的返回值
 * 知识点：
 *   返回类型是 void：函数不交回结果。
 *   返回类型是 int、double 这些：函数体里要用 return 交回一个同类型的值。
 *   return 后面的类型和函数声明不一致时，会发生类型转换。
 *     (x + y) / 2 是两个 int 相除，先得到整数 1，再变成 double 1.00，小数已经丢了。
 *     return 100.123 交给 int 函数，小数会被截掉，变成 100。
 *   声明了非 void 的返回类型，却没有任何 return，调用后得到什么是不确定的。这种函数不要写。
 */
#include <stdio.h>

void func(void)
{
    printf("无返回值\n");
}

int get_min(int a, int b)
{
    return a < b ? a : b;
}

double get_avg_truncated(int x, int y)
{
    return (x + y) / 2;
}

double get_avg(int x, int y)
{
    return (x + y) / 2.0;
}

int truncated_max(void)
{
    return (int)100.123;
}

int main(void)
{
    func();
    printf("最小值: %d\n", get_min(10, 20));
    printf("整数除法的平均值: %.2f\n", get_avg_truncated(1, 2));
    printf("浮点除法的平均值: %.2f\n", get_avg(1, 2));
    printf("截断后的返回值: %d\n", truncated_max());
    return 0;
}
