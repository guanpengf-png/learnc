/*
 * 章节：指针运算
 * 知识点：
 *   指针加一个整数，不是地址的数值加这个整数，而是往后跳过这么多个元素。
 *   跳过的字节数 = 整数 * 这个类型占的字节数。int 通常是 4，short 通常是 2。
 *   自增、自减就是加 1、减 1。
 *   同一段数组里的两个同类型指针可以相减，结果是它们隔着几个元素，类型是 ptrdiff_t，用 %td 打印。
 *   高地址减低地址得到正数。两个互不相关的变量做指针相减没有定义，不要写。
 *   数组下标写法 a[i] 就是 *(a + i)。所以 i[a] 也能用，只是不好读。
 */
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    int nums[] = {10, 20, 30, 40, 50};
    int *ptr = &nums[0];
    printf("一开始 *ptr = %d\n", *ptr);
    ptr += 3;
    printf("加 3 之后 *ptr = %d\n", *ptr);
    ptr -= 2;
    printf("再减 2 之后 *ptr = %d\n", *ptr);

    short shorts[] = {10, 20, 30, 40, 50};
    int len = (int)(sizeof shorts / sizeof shorts[0]);
    short *cursor = &shorts[0];
    for (int i = 0; i < len; i++) {
        printf("正向 [%d] 地址 %p 值 %d\n", i, (void *)cursor, *cursor);
        cursor++;
    }
    for (int i = len - 1; i >= 0; i--) {
        cursor--;
        printf("反向 [%d] 地址 %p 值 %d\n", i, (void *)cursor, *cursor);
    }

    int *begin = &nums[0];
    int *fourth = &nums[3];
    printf("fourth - begin = %td\n", fourth - begin);
    printf("begin - fourth = %td\n", begin - fourth);

    printf("下标的几种等价写法，第 2 个元素:\n");
    printf("%d %d %d %d\n", nums[2], *(begin + 2), *(nums + 2), 2[nums]);
    return 0;
}
