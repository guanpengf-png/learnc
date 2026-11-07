/*
 * 章节：指针数组和数组指针
 * 知识点：
 *   指针数组：一个数组，每个元素都是指针。int *arr[3];
 *     方括号优先，先看成数组，元素类型是 int *。
 *   数组指针：一个指针，指向一整段数组。int (*ptr)[5];
 *     圆括号不能省。省掉就变成指针数组。
 *   两者存的地址数值可能一样，加 1 之后差很多：
 *     指向元素的指针加 1，跳过一个元素。
 *     指向数组的指针加 1，跳过整段数组。
 *   用数组指针取元素，要先解引用：(*ptr)[i]。
 */
#include <stdio.h>

int main(void)
{
    int num1 = 10;
    int num2 = 20;
    int num3 = 30;
    int *arr[3];
    arr[0] = &num1;
    arr[1] = &num2;
    arr[2] = &num3;
    for (int i = 0; i < 3; i++) {
        printf("指针数组 [%d] 指向的值 = %d\n", i, *arr[i]);
    }

    int values[5] = {10, 20, 30, 40, 50};
    int (*ptr)[5] = &values;
    printf("values    = %p\n", (void *)values);
    printf("ptr       = %p\n", (void *)ptr);
    printf("values+1  = %p\n", (void *)(values + 1));
    printf("ptr+1     = %p\n", (void *)(ptr + 1));
    for (int i = 0; i < 5; i++) {
        printf("(*ptr)[%d] = %d\n", i, (*ptr)[i]);
    }
    return 0;
}
