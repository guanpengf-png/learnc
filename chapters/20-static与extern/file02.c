/*
 * 章节：extern 声明外部定义
 * 知识点：
 *   别的文件里定义的全局名字，本文件要先 extern 声明，才能使用。
 *   extern 只是声明“它在别处定义”，不会再分配一份新空间。
 *   static 全局名字不能跨文件使用。下面被注释掉的声明如果打开并使用，链接会报 undefined symbol。
 *   和 file01.c 一起编译，不要单独编译本文件。
 */
#include <stdio.h>

extern int a;
extern const double PI;
extern char chs[];
extern void func(void);
extern void call_static(void);

/* 这些在 file01.c 里是 static，跨文件不可见：
 * extern int a1;
 * extern char chs1[];
 * extern void func1(void);
 */

int main(void)
{
    printf("main function\n");
    printf("a = %d, PI = %.2f, chs = %s\n", a, PI, chs);
    func();
    call_static();
    return 0;
}
