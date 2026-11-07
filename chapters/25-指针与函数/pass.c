/*
 * 章节：把指针和数组传给函数
 * 知识点：
 *   形参是指针时，调用处传地址、指针，或者数组名。
 *   函数里 *p = ... 改的是调用者那块内存，所以调用者能看到变化。
 *   数组名传到函数里会变成指向首元素的指针。函数参数里的 int arr[] 和 int *arr 是一回事。
 *   函数里对这个指针做 ++，改的是函数自己的那份地址副本，不会改掉调用者的数组名。
 *   但通过这份地址写进去的元素，调用者的数组会跟着变。
 */
#include <stdio.h>

static void bump(int *p)
{
    *p += 1;
}

static double average(int *arr, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += *arr;
        arr++;
    }
    return (double)sum / size;
}

int main(void)
{
    int num = 100;
    int *ptr = &num;
    bump(&num);
    printf("传入地址之后 num = %d\n", num);
    bump(ptr);
    printf("传入指针之后 num = %d\n", num);

    int balance[5] = {1000, 2, 3, 17, 50};
    printf("平均值 = %.2f\n", average(balance, 5));
    printf("数组本身还在: %d\n", balance[0]);
    return 0;
}
