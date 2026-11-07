/*
 * 章节：带参数的宏
 * 知识点：
 *   #define 名字(形参) 替换文本
 *   宏名和左括号之间不能有空格，否则空格后面的内容都会变成替换文本。
 *   形参没有类型，也不会单独分配内存。调用时用实参把形参原样换掉。
 *   替换文本里的形参和整个表达式都要加括号。
 *     SQ_BAD(a + 2) 变成 a + 2 * a + 2，乘法先做，结果不是平方。
 *     SQ(a + 2) 变成 (a + 2) * (a + 2)。
 *   宏不是函数。它在编译前展开，不会对参数先求一次值。
 *   SQ(i++) 会变成 (i++) * (i++)，同一个变量被修改两次，这是未定义行为，不能写进会运行的程序。
 *   函数参数只求值一次，所以 square(i++) 只会让 i 增加 1。
 */
#include <stdio.h>

#define SQ_BAD(x) x * x
#define SQ(x) ((x) * (x))

static int square(int x)
{
    return x * x;
}

int main(void)
{
    int a = 3;
    printf("不加括号: %d\n", SQ_BAD(a + 2));
    printf("加上括号: %d\n", SQ(a + 2));

    int i = 1;
    printf("函数只求值一次: %d\n", square(i++));
    printf("调用函数之后 i = %d\n", i);
    return 0;
}
