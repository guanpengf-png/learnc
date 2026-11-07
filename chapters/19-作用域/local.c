/*
 * 章节：局部作用域
 * 知识点：
 *   函数内部定义的变量、常量、数组，以及这个函数的形参，都只能在这个函数里用。
 *   函数外面写它们的名字，会编译失败。
 *   每调用一次函数，就新做一份局部变量；函数返回，这份局部变量就销毁。
 */
#include <stdio.h>

void show_value(int a)
{
    double pi = 3.1415;
    int nums[3] = {10, 20, 30};
    printf("a = %d, PI = %.2f\n", a, pi);
    for (int i = 0; i < (int)(sizeof nums / sizeof nums[0]); i++) {
        printf("nums[%d] = %d ", i, nums[i]);
    }
    printf("\n");
}

int main(void)
{
    show_value(100);
    /* 下面两行在函数外面使用局部名字，不能编译：
     * printf("%d %f\n", a, pi);
     * printf("%d\n", nums[0]);
     */
    return 0;
}
