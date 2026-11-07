/*
 * 章节：数组名和指针的差别
 * 知识点：
 *   数组名在表达式里通常会变成“指向首元素的指针”。所以数组名和 &arr[0] 的地址相同。
 *   有三处不要把数组名当成真正的指针变量：
 *     sizeof 数组名 是整个数组的字节数；sizeof 指针 是指针自己的字节数。
 *     数组名不能自增、自减。
 *     数组名不能改去指向别的地方。指针变量可以。
 */
#include <stdio.h>

int main(void)
{
    int nums[5] = {10, 20, 30, 40, 50};
    int *ptr = &nums[0];

    printf("nums = %p, *nums = %d\n", (void *)nums, *nums);
    printf("ptr  = %p, *ptr  = %d\n", (void *)ptr, *ptr);
    printf("%s\n", ptr == nums ? "地址相同" : "地址不同");
    printf("sizeof nums = %zu, sizeof ptr = %zu\n", sizeof nums, sizeof ptr);

    ptr++;
    printf("指针自增后 *ptr = %d\n", *ptr);
    /* nums++;  数组名不能自增。 */

    int n = 100;
    ptr = &n;
    printf("指针改指向 n 之后 *ptr = %d\n", *ptr);
    /* nums = &n;  数组名不能被重新赋值。 */
    return 0;
}
