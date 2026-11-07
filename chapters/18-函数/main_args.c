/*
 * 章节：main 的参数
 * 知识点：
 *   int main(void) 是没有命令行参数的写法。
 *   int main(int argc, char *argv[]) 用来接收命令行参数。
 *   argc 是参数个数，至少为 1。argv[0] 是程序自己的名字，从 argv[1] 开始才是你额外传入的字符串。
 *   运行示例：./main_args apple banana
 *   main 返回 0 表示成功，返回别的非 0 整数表示失败。即使省略 return 0，编译器也会补上，但建议自己写。
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
