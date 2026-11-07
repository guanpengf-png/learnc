/*
 * 章节：malloc、calloc、realloc、free
 * 知识点：
 *   局部变量在栈上，全局和 static 在静态区。这两块的大小在编译或进入函数时就定了。
 *   堆上的内存用 malloc、calloc、realloc 按运行时的需要申请，用完必须 free。
 *   申请到的内存没有名字，只能用指针保存地址。
 *   malloc(字节数) 不负责清零。calloc(个数, 每个多大) 会把每个字节设成 0。
 *   realloc(原指针, 新字节数) 调整大小。地址可能变，也可能不变，所以一定要用返回值覆盖原指针。
 *   失败时 realloc 返回 NULL，原来的那块仍然有效，不要弄丢。
 *   free 之后不能再读那块内存，也不能对同一个地址 free 第二次。习惯上 free 之后把指针设成 NULL。
 *   申请失败会返回 NULL，要先判断再使用。
 *   void * 可以指向任何类型，但不能直接解引用。用之前要转换成具体类型的指针。
 *   别的指针赋给 void * 可以隐式转换。void * 赋回具体指针时，建议写成显式转换。
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int num = 10;
    double d = 12.345;
    void *vp1 = &num;
    void *vp2 = &d;
    printf("void* 转回 int: %d\n", *(int *)vp1);
    printf("void* 转回 double: %f\n", *(double *)vp2);

    int *one = malloc(sizeof *one);
    if (one == NULL) {
        printf("分配失败\n");
        return 1;
    }
    *one = 100;
    printf("单个 int: %d\n", *one);
    free(one);
    one = NULL;

    int len = 5;
    int *arr = malloc((size_t)len * sizeof *arr);
    if (arr == NULL) {
        printf("分配数组失败\n");
        return 1;
    }
    for (int i = 0; i < len; i++) {
        arr[i] = i * 10;
    }
    printf("malloc 的数组:");
    for (int i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");

    int *cleared = calloc((size_t)len, sizeof *cleared);
    if (cleared == NULL) {
        free(arr);
        return 1;
    }
    printf("calloc 刚分配时:");
    for (int i = 0; i < len; i++) {
        printf(" %d", cleared[i]);
    }
    printf("\n");
    free(cleared);

    int *grown = realloc(arr, (size_t)len * 2 * sizeof *arr);
    if (grown == NULL) {
        free(arr);
        return 1;
    }
    arr = grown;
    printf("realloc 之后原来的前 5 个仍然是:");
    for (int i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
    free(arr);
    arr = NULL;
    return 0;
}
