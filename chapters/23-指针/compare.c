/*
 * 章节：指针比较
 * 知识点：
 *   ==、!= 比较两个指针是不是同一个地址。不同类型也可以比相等，结果只表示地址相同或不同。
 *   <、>、<=、>= 只能用在同一段数组里的指针上，比的是谁在更前面。
 *   两个不相关对象的指针做大小比较，没有定义。讲义里 int 指针和 double 指针比大小，不能当成确定结果。
 */
#include <stdio.h>

int main(void)
{
    int nums[] = {10, 20, 30, 40, 50};
    int *first = &nums[0];
    int *fourth = &nums[3];
    int *again = &nums[0];

    printf("first < fourth : %d\n", first < fourth);
    printf("first > fourth : %d\n", first > fourth);
    printf("first == again : %d\n", first == again);
    printf("first != fourth : %d\n", first != fourth);
    return 0;
}
