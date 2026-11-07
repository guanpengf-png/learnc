/*
 * 章节：const 指针、多级指针、空指针和野指针
 * 知识点：
 *   const 在 * 左边：不能通过这个指针改它指向的内容，但指针自己可以改去指向别处。
 *     const int *p 和 int const *p 是同一种。
 *   const 在 * 右边：指针自己不能改指向，但可以改它指向的内容。int *const p。
 *   两边都有 const：指向不能改，内容也不能通过它改。
 *   多级指针：int **pp 里存的是另一个指针的地址。有几级，解引用就写几个 *。
 *   空指针：值为 NULL 的指针，表示“现在不指向任何对象”。使用前要判断是不是 NULL。
 *   野指针指向一块你不能用的内存。常见来源：
 *     定义后没有初始化。
 *     自增之后越过了数组。
 *     指向的局部变量已经随函数返回被销毁。
 *   避免方法：定义时先写成 NULL；使用前检查；不要返回局部变量地址；不要越界。
 */
#include <stdio.h>

int main(void)
{
    int num1 = 10;
    int num2 = 20;
    const int *readable = &num1;
    printf("const 指针读到 %d\n", *readable);
    readable = &num2;
    printf("它可以改指向，现在读到 %d\n", *readable);
    /* *readable = 1;  不能通过它改内容。 */

    int *const fixed = &num1;
    *fixed = 100;
    printf("指针 const 改了内容，num1 = %d\n", num1);
    /* fixed = &num2;  不能改它的指向。 */

    int var = 3000;
    int *ptr = &var;
    int **pptr = &ptr;
    int ***ppptr = &pptr;
    printf("一级 %d, 二级 %d, 三级 %d\n", *ptr, **pptr, ***ppptr);

    int *empty = NULL;
    if (empty == NULL) {
        printf("使用前是空指针\n");
    }
    empty = &num2;
    printf("赋值之后 *empty = %d\n", *empty);
    return 0;
}
