/*
 * 章节：宏定义
 * 知识点：
 *   预处理发生在编译之前。以 # 开头的行是预处理指令，不加分号，一般独占一行。
 *   #define 名字 替换文本
 *   后面代码里出现这个名字，会被原样换成那段文本。预处理不检查这段文本对不对，错误要等编译才发现。
 *   宏名习惯全大写，好和变量区分。
 *   替换文本里可以继续用已经定义的宏，预处理会一层层展开。
 *   #undef 名字 用来取消这个宏。取消之后再写这个名字，就不再替换。
 *   讲义用“如果 / 否则 / 输出”替换 if / else / printf，说明宏连关键字都能换。
 *   那只是文本替换，不会变成一门新语言。为了让示例在严格编译选项下稳定，这里不用中文做宏名。
 */
#include <stdio.h>

#define PI 3.14
#define BOOL int
#define TRUE 1
#define FALSE 0
#define S (PI * y * y)
#define PRINT_SUM printf("sum = %d\n", sum)
#define M (n * n + n * 3)

static void before_undef(void)
{
    printf("取消之前 PI = %f\n", PI);
}

int main(void)
{
    before_undef();

    BOOL is_ok = TRUE;
    BOOL is_pass = FALSE;
    printf("结果为: %s\n", is_ok ? "ok" : "pass");
    printf("is_pass = %d\n", is_pass);

    int y = 2;
    printf("圆面积相关的展开结果: %f\n", S);

    int n = 10;
    int sum = 3 * M + 4 * M + 5 * M;
    PRINT_SUM;
    return 0;
}

#undef PI
/* #undef 之后再写 PI，预处理不会替换，编译会说 PI 未声明。 */
