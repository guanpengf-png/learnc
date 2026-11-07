/*
 * 章节：静态局部变量
 * 知识点：
 *   static 加在局部变量前面，这个变量就放进全局静态区，而不是栈。
 *   它只在第一次进入函数时初始化一次。函数返回后也不销毁，下次调用还是上次留下的值。
 *   没写初值的静态局部变量会自动变成 0，和全局变量一样。
 *   普通局部变量每次调用都重新创建。没写初值的普通局部变量是垃圾值，不要读取。
 *   static 并不扩大可见范围：别的函数仍然不能直接写这个名字。
 */
#include <stdio.h>

void fn(void)
{
    int n = 10;
    printf("普通局部 n = %d\n", n);
    n++;
    printf("函数内部 n++ 之后 = %d，返回后这份 n 就没了\n", n);
}

void fn_static(void)
{
    static int n = 10;
    static int a;
    printf("静态 n = %d, 未赋初值的静态 a = %d\n", n, a);
    n++;
    printf("静态 n++ 之后 = %d\n", n);
}

int main(void)
{
    fn();
    fn_static();
    printf("---- 再调用一次 ----\n");
    fn();
    fn_static();
    return 0;
}
