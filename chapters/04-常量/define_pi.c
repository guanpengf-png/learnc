/*
 * 章节：用 #define 定义常量
 * 知识点：
 *   #define 是预处理指令，编译前做文本替换，不检查类型。
 *   末尾不要写分号，否则分号会变成被替换进去的内容。
 *   写在函数外面，后面的代码都能用。
 *   习惯上常量名全大写，和变量区分。
 *   %.2f 表示按浮点数输出，保留 2 位小数。
 *   scanf 读 double 必须用 %lf；printf 里 %f 和 %lf 都可以。
 *
 * 试运行输入：2.0
 * 期望：圆的面积为:12.57
 */
#include <stdio.h>

#define PI 3.1415

int main(void)
{
    double r;
    printf("请您输入一个圆的半径 \n");
    if (scanf("%lf", &r) != 1) {
        return 1;
    }

    double s = PI * r * r; /* 预处理后这里相当于 3.1415 * r * r */
    printf("圆的面积为:%.2f\n", s);
    return 0;
}
