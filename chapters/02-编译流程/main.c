/*
 * 章节：C 程序从源码到运行
 * 知识点：CPU 只认识机器码。高级语言要经过四步才变成可执行文件。
 *
 *   源文件 main.c
 *     |  预处理 cc -E    处理 #include、删注释，得到 main.i
 *     |  编译   cc -S    翻译成汇编 main.s
 *     |  汇编   cc -c    翻译成目标文件 main.o（机器码，还不能直接运行）
 *     |  链接   cc       把 main.o 和 C 标准库拼起来，得到可执行文件
 *     v
 *   运行 ./main
 *
 * <stdio.h> 只是声明。printf 的真正实现在标准库里，链接时才会接上。
 * 同目录的 steps.sh 会把这四个中间文件都留下来。
 */
#include <stdio.h>

int main(void)
{
    printf("Hello World\n");
    return 0;
}
