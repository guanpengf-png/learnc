/*
 * 章节：返回指针的函数，以及函数指针
 * 知识点：
 *   返回类型写成 类型 *函数名(...)，这个函数返回的是地址，叫指针函数。
 *   不能返回局部变量的地址。函数一返回，局部变量就销毁了，调用者拿到的是野指针。
 *   可以返回静态局部变量的地址，也可以返回调用者传进来的那块内存的地址，或者字符串字面量的地址。
 *   函数指针是指向函数的指针：返回类型 (*名字)(参数列表)。
 *   圆括号不能省。写成 int *f(int) 就变成“返回 int 指针的函数”。
 *   调用时 (*fp)(参数) 和 fp(参数) 都可以。
 *   回调就是把函数指针传进另一个函数，让那个函数在合适的时候调用它。
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *longer(char *left, char *right)
{
    return strlen(left) > strlen(right) ? left : right;
}

static int *static_max(int x, int y)
{
    static int result;
    result = x > y ? x : y;
    return &result;
}

static int *rand_array(void)
{
    static int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = rand() % 50;
    }
    return arr;
}

static int get_sum(int x, int y)
{
    return x + y;
}

static int double_num(int num)
{
    return num * 2;
}

static void apply(int *arr, int len, int (*fn)(int))
{
    for (int i = 0; i < len; i++) {
        arr[i] = fn(arr[i]);
    }
}

int main(void)
{
    char left[] = "cat";
    char right[] = "banana";
    printf("更长的是 %s\n", longer(left, right));

    int *max_num = static_max(10, 20);
    printf("静态变量里的最大值 = %d\n", *max_num);

    int *numbers = rand_array();
    printf("随机数:");
    for (int i = 0; i < 10; i++) {
        printf(" %d", numbers[i]);
    }
    printf("\n");

    int (*ptr_sum)(int, int) = get_sum;
    printf("函数指针调用: %d\n", (*ptr_sum)(10, 20));
    printf("省略星号的调用: %d\n", ptr_sum(30, 40));

    int arr[] = {1, 2, 3, 4, 5};
    apply(arr, 5, double_num);
    printf("翻倍后:");
    for (int i = 0; i < 5; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
    return 0;
}
