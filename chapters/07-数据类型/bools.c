/*
 * 章节：布尔类型
 * 知识点：
 *   C89 没有布尔类型。0 是假，非 0 是真。可以用宏把 int 伪装成布尔。
 *   C99 的 _Bool 只能保存 0 或 1，赋给它的非 0 值都会变成 1。
 *   <stdbool.h> 提供更好记的名字：bool、true、false。日常用这一种。
 */
#include <stdio.h>
#include <stdbool.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

int main(void)
{
    BOOL b1 = TRUE;
    BOOL b2 = FALSE;
    printf("宏定义: b1 = %d, b2 = %d\n", b1, b2);
    if (b1) {
        printf("b1 is true\n");
    }
    if (!b2) {
        printf("b2 is false\n"); /* b2 本身是 0，取反后条件才成立 */
    }

    _Bool n1 = 0;
    _Bool n2 = 1;
    _Bool n3 = -5; /* 非 0，存进去变成 1 */
    printf("_Bool: n1 = %d, n2 = %d, n3 = %d\n", n1, n2, n3);

    bool flag_false = false;
    bool flag_true = true;
    printf("stdbool: false = %d, true = %d\n", flag_false, flag_true);
    return 0;
}
