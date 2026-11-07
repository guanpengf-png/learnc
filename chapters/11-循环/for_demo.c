/*
 * 章节：for 循环
 * 知识点：
 *   for (初始化; 条件; 更新) { 循环体 }
 *   初始化只执行一次。每次先判断条件，为真才执行循环体，然后执行更新。
 *   三部分都可以省略，但两个分号不能省。
 *   for(;;) 是死循环。需要时放在注释里理解，不要真的写进会运行的程序。
 *   可以同时写多个循环变量，用逗号隔开。
 */
#include <stdio.h>

int main(void)
{
    for (int i = 0; i < 10; i++) {
        printf("我第 %d 天吃了 %d 个包子\n", i, i);
    }

    printf("7 到 15: ");
    for (int i = 7; i <= 15; i++) {
        printf("%d ", i);
    }
    printf("\n");

    printf("56 到 43: ");
    for (int i = 56; i >= 43; i--) {
        printf("%d ", i);
    }
    printf("\n");

    printf("偶数: ");
    for (int i = 0; i <= 10; i += 2) {
        printf("%d ", i);
    }
    printf("\n");

    int sum = 0;
    for (int i = 0; i <= 100; i++) {
        sum += i;
    }
    printf("0 到 100 的和 = %d\n", sum);

    int product = 1;
    for (int i = 1; i < 10; i += 2) {
        product *= i;
    }
    printf("10 以内奇数的乘积 = %d\n", product);

    int j = 0;
    for (; j < 3; j++) {
        printf("循环变量写在外面, j = %d\n", j);
    }

    for (int m = 0, n = 0; m < 5 && n < 3; m++, n++) {
        printf("两个循环变量: %d, %d\n", m, n);
    }
    return 0;
}
