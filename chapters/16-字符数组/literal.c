/*
 * 章节：用字符串字面量初始化字符数组
 * 知识点：
 *   char s1[] = {"what"}; 和 char s2[] = "what"; 是一回事。
 *   双引号字符串会自动在末尾加 '\0'。
 *   "what" 这个数组的长度是 5，不是 4。
 */
#include <stdio.h>

int main(void)
{
    char chs1[] = {"what are you no sha lei"};
    char chs2[] = "no sha lei ni shuo ne";

    printf("%s\n", chs1);
    printf("%s\n", chs2);
    printf("\"love\" 的数组长度 = %zu\n", sizeof "love");
    return 0;
}
