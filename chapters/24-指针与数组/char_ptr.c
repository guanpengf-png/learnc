/*
 * 章节：字符指针
 * 知识点：
 *   char *s = "hello"; 把指针指向字符串字面量的首字符。字面量本身放在静态区。
 *   字符数组 char buf[] = "hello"; 会把字符复制到数组自己的内存里。
 *   数组名不能改去指向另一段字符串，只能改里面的字符。
 *   字符指针可以改去指向另一段字面量。不要通过这个指针去改字面量里的字符，那是未定义行为。
 */
#include <stdio.h>

int main(void)
{
    char buf[14] = "hello tom";
    buf[0] = 'H';
    printf("数组里的字符可以改: %s\n", buf);
    /* buf = "tom";  数组名不能重新赋值。 */

    char *text = "hello tom";
    printf("指针现在指向: %s\n", text);
    text = "tom";
    printf("指针可以改指向: %s\n", text);

    char *walk = "hello";
    for (int i = 0; walk[i] != '\0'; i++) {
        printf("[%d] = %c, 地址 %p\n", i, walk[i], (void *)&walk[i]);
    }
    return 0;
}
