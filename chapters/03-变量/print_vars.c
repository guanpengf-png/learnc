/*
 * 章节：用 printf 输出变量
 * 知识点：
 *   printf 的名字来自 print formatted，按格式输出。
 *   双引号里的字符串叫格式化字符串。
 *   %d 是整数占位符，后面的参数按顺序填进去，一一对应。
 *   普通字符（例如「num1的值为」）会原样输出。
 */
#include <stdio.h>

int main(void)
{
    int num1;
    num1 = 100;

    int num2 = 200;
    printf("num1的值为%d \n", num1);
    printf("num2的值为%d \n", num2);

    num2 = num1; /* 把 num1 的 100 抄给 num2 */
    printf("num2的值为%d \n", num2);

    int a = 10, b = 20, c = 30;
    printf("%d %d %d\n", a, b, c);
    return 0;
}
