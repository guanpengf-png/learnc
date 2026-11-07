/*
 * 章节：可变参数
 * 知识点：
 *   参数个数事先不确定时，可以在至少一个固定参数后面写 ...
 *   读取要用 <stdarg.h>：
 *     va_list 保存“下一个可变参数在哪里”。
 *     va_start(ap, 最后一个固定参数) 让它指向第一个可变参数。
 *     va_arg(ap, 类型) 取出下一个，类型必须和调用时真正传入的类型一致。
 *     va_end(ap) 用完后清理。
 *   vprintf 和 printf 一样，只是后面那串参数已经装在 va_list 里。
 *   讲义里的 get_min 虽然写了 ...，函数体却没有读取第三个参数。个数和类型都要由你自己约定，编译器不会帮你检查。
 */
#include <stdarg.h>
#include <stdio.h>

static int get_min(int count, ...)
{
    va_list args;
    va_start(args, count);
    int min = va_arg(args, int);
    for (int i = 1; i < count; i++) {
        int num = va_arg(args, int);
        if (num < min) {
            min = num;
        }
    }
    va_end(args);
    return min;
}

static void show_msg(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

int main(void)
{
    printf("最小值: %d\n", get_min(3, 10, 20, 30));
    show_msg("分别是: %s, %s\n", "hello", "world");
    return 0;
}
