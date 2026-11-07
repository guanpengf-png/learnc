/*
 * 章节：math.h 里的数学函数
 * 知识点：
 *   这一组函数的参数和返回值都是 double，除了 abs。abs 处理 int，声明在 <stdlib.h>。
 *   fabs 绝对值，sqrt 平方根，cbrt 立方根，pow(x, y) 是 x 的 y 次方。
 *   ceil 向正无穷取整，floor 向负无穷取整。负数时这两个方向相反，不要记成“去掉小数”。
 *   trunc 是直接砍掉小数，round 是四舍五入。
 *   讲义把 floor(3.124) 印成 4、把 floor(-3.124) 印成 -3，都不对。
 *   floor(3.124) 是 3，floor(-3.124) 是 -4。ceil(-3.124) 才是 -3。
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("abs(-2) = %d\n", abs(-2));
    printf("fabs(-2.0) = %.2f\n", fabs(-2.0));
    printf("sqrt(9) = %.2f\n", sqrt(9));
    printf("cbrt(8) = %.2f\n", cbrt(8));
    printf("pow(3, 4) = %.2f\n", pow(3, 4));
    printf("ceil(3.124) = %.0f\n", ceil(3.124));
    printf("ceil(-3.124) = %.0f\n", ceil(-3.124));
    printf("floor(3.124) = %.0f\n", floor(3.124));
    printf("floor(-3.124) = %.0f\n", floor(-3.124));
    printf("round(2.56) = %.0f\n", round(2.56));
    printf("trunc(12.91) = %.0f\n", trunc(12.91));
    return 0;
}
