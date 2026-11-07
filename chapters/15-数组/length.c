/*
 * 章节：用 sizeof 计算数组长度
 * 知识点：
 *   数组长度在定义时就定了。C 没有一个运行时函数能直接问“这个数组有几个元素”。
 *   元素个数 = 整个数组的字节数 / 一个元素的字节数。
 *   sizeof 数组名 得到整个数组的字节数。数组名作为函数参数传进去之后，这条规则就失效了，
 *   函数里拿到的只是首元素地址，sizeof 得到的是指针的大小。所以长度要在定义它的那个函数里算，再传给别的函数。
 */
#include <stdio.h>

int main(void)
{
    int arr[] = {10, 20, 30, 40, 50};

    int arr_byte_length = (int)sizeof arr;
    int element_byte_length = (int)sizeof arr[0];
    int arr_length = arr_byte_length / element_byte_length;
    printf("数组的元素个数为: %d\n", arr_length);

    int arr_length1 = (int)(sizeof arr / sizeof arr[0]);
    printf("简化后的元素个数: %d\n", arr_length1);
    printf("整个数组 %d 字节，一个 int %d 字节\n", arr_byte_length, element_byte_length);
    return 0;
}
