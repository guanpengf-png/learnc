/*
 * 章节：年轻人的第一个 C 程序
 * 知识点：
 *   1. 源文件扩展名是 .c
 *   2. #include <stdio.h> 引入标准输入输出库，printf 在这里声明
 *   3. 程序从 main 进入。int main(void) 表示不接收参数，并返回 int
 *   4. 每条语句以分号结束
 *   5. return 0 告诉操作系统：正常结束
 *
 * 编译运行：
 *   cc -std=c11 -Wall -Wextra -Wpedantic hello.c -o hello && ./hello
 */
#include <stdio.h>

int main(void)
{
    printf("hello,world!\n");
    return 0;
}
