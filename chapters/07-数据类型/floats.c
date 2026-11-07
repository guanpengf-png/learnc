/*
 * 章节：浮点类型
 * 知识点：
 *   float 约 6~7 位有效数字，double 约 15~16 位。开发中优先用 double。
 *   小数写出来默认是 double。float 要加后缀 f 或 F，long double 加 L。
 *   科学计数法：5.12e2 是 512，5.12E-2 是 0.0512。
 *   printf：%f 默认 6 位小数，%.2f 保留 2 位；%e 用科学计数法。
 *   scanf：float 用 %f，double 必须用 %lf，long double 必须用 %Lf。
 *   浮点数按 IEEE 754 存成符号、指数、尾数，所以很多十进制小数不能精确表示。
 *   下面 f3 的输出会出现一长串 9 或 0，这是精度损失，不是 printf 算错。
 */
#include <stdio.h>

int main(void)
{
    float f1 = 0.12345f;
    float f2 = 3.11592f;
    float f3 = -2e12f;
    float f4 = 1.9823e2f;
    printf("f1 = %f, f2 = %f, f3 = %.2f, f4 = %f\n", (double)f1, (double)f2, (double)f3, (double)f4);
    printf("f1 = %e, f2 = %.2e, f3 = %e, f4 = %e\n", (double)f1, (double)f2, (double)f3, (double)f4);

    double d1 = 3.1415926;
    double d2 = 0.123456;
    double d3 = -2e12;
    double d4 = 1.9823e2;
    printf("d1 = %.15f\n", d1); /* 需要更多小数位时，写在 f 前面 */
    printf("d2 = %.2f, d3 = %f, d4 = %f\n", d2, d3, d4);
    printf("d1 = %e, d2 = %.2e, d3 = %e, d4 = %e\n", d1, d2, d3, d4);
    return 0;
}
