/*
 * 章节：不确定次数的循环
 * 知识点：
 *   次数事先不知道时，用输入值做条件，而不是写死循环次数。
 *   while 先判断再执行。变量在判断前必须已经有值。
 *   讲义里的 while (pwd != 12345) 没有给 pwd 初值，这是未定义行为，不要照抄。
 *   do...while 先执行一次再判断，所以第一次输入前可以还没有密码。
 *   do...while 的 while 后面那个分号不能省。
 *
 * 下面用一组预先写好的输入代替键盘，方便直接看到循环怎么停。
 * 真正从键盘读时，把数组那几行换成 scanf("%d", &pwd)。
 */
#include <stdio.h>

int main(void)
{
    int attempts[] = {11111, 22222, 12345};
    int count = (int)(sizeof attempts / sizeof attempts[0]);
    int index = 0;
    int pwd = 0; /* 先给一个肯定不等于密码的值，while 才能合法地开始判断 */

    while (pwd != 12345) {
        pwd = attempts[index];
        index++;
        printf("while 读到密码 %d\n", pwd);
        if (index >= count) {
            break;
        }
    }

    /*
     * 下面这个 do...while 即使第一次就输入正确，循环体也已经执行过一次。
     * 如果改成 while，并且一开始 pwd 就是 123，循环体一次都不会执行。
     */
    pwd = 123;
    int runs = 0;
    do {
        runs++;
        printf("do...while 第 %d 次执行，当前密码 %d\n", runs, pwd);
    } while (pwd != 123);
    printf("密码一开始就正确，do...while 仍然执行了 %d 次\n", runs);
    return 0;
}
