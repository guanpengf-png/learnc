/*
 * 章节：文件包含和条件编译
 * 知识点：
 *   #include <标准库头文件> 从系统目录找。
 *   #include "自己的头文件" 先从当前源文件所在目录找。上级目录写成 ../文件名.h。
 *   头文件里放宏、类型、函数声明。用 #ifndef / #define / #endif 防止同一个头文件被包含两次。
 *   #if 0 ... #endif 常用来临时关掉一段代码。条件是非 0 才保留里面的内容。
 *   #ifdef 名字 表示这个宏已经定义过，#ifndef 表示还没定义。
 *   没定义过的宏，在 #if 里当成 0。
 *   #elif 必须写在 #else 前面。讲义里的 #if define 少了一个 d，正确写法是 #if defined。
 *   __STDC_VERSION__ 表示语言标准。C99 是 199901L，不是讲义里的 199001L。C11 是 201112L。
 *   跨平台暂停：Windows 用 windows.h 里的 Sleep，单位是毫秒；
 *   Linux 用 unistd.h 里的 sleep，单位是秒；macOS 同样用 sleep。
 *   下面只判断当前平台，不真的暂停。
 */
#include <stdio.h>

#include "greet.h"
#include "greet.h"

int main(void)
{
#if 0
    printf("这段被预处理器丢掉了\n");
#endif

#ifdef LEARN_C_ADV
    printf("定义了 LEARN_C_ADV\n");
#else
    printf("没有定义 LEARN_C_ADV\n");
#endif

#ifndef LEARN_C_ADV
    printf("确认这个宏还没定义\n");
#endif

    printf("__STDC_VERSION__ = %ld\n", __STDC_VERSION__);

#if defined(_WIN32)
    const char *platform = "windows";
#elif defined(__APPLE__)
    const char *platform = "apple";
#elif defined(__linux__)
    const char *platform = "linux";
#else
    const char *platform = "other";
#endif
    printf("当前平台: %s\n", platform);
    greet();
    return 0;
}
