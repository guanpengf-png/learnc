/*
 * 章节：指针是什么
 * 知识点：
 *   内存按字节编号，这个编号就是地址。变量的地址，是它占用的第一个字节的地址。
 *   专门存放地址的变量叫指针。指针指向谁，就可以间接读写谁。
 *   定义：类型 *名字。星号靠近类型还是靠近名字都可以，意思一样。
 *   &变量 取出地址。*指针 取出它指向的值，也叫解引用。
 *   打印地址用 %p，参数要转成 void *。
 *   指针自己也有地址。指针里存的地址，和指针这个变量所在的地址，不是同一个东西。
 *   指针占几个字节取决于系统：64 位系统上一般是 8 字节。
 */
#include <stdio.h>

int main(void)
{
    int num = 100;
    int *p1 = &num;
    int *p2 = &num;
    int *p3 = &num;

    printf("num = %d, &num = %p\n", num, (void *)&num);
    printf("p1 里存的地址 = %p\n", (void *)p1);
    printf("p1 自己的地址 = %p\n", (void *)&p1);
    printf("*p1 = %d\n", *p1);
    printf("三种写法指向同一处: %d %d %d\n", *p1, *p2, *p3);
    printf("指针占 %zu 字节\n", sizeof p1);

    double value = 2.88;
    double *a = &value;
    double *b = a;
    printf("修改前 %.2f\n", value);
    *a = 3.88;
    printf("通过 a 修改后 %.2f\n", value);
    *b += 10;
    printf("通过 b 再加 10 后 %.2f\n", value);
    return 0;
}
