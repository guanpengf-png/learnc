/*
 * 章节：访问数组元素，以及越界
 * 知识点：
 *   用 数组名[下标] 读取或修改元素。下标从 0 开始，最后一个合法下标是 长度 - 1。
 *   合法范围之外都叫越界，包括太大的下标和负数下标。
 *   C 不检查越界。越界读到的值不确定，也可能直接让程序崩溃。这种代码不能拿来运行。
 */
#include <stdio.h>

int main(void)
{
    int arr[3] = {10, 20, 30};
    arr[1] += 100;

    printf("第一个元素: %d\n", arr[0]);
    printf("第二个元素: %d\n", arr[1]);
    printf("第三个元素: %d\n", arr[2]);

    int sample[] = {10, 20, 30, 40, 50};
    int len = (int)(sizeof sample / sizeof sample[0]);
    printf("合法下标是 0 到 %d\n", len - 1);
    for (int i = 0; i < len; i++) {
        printf("下标 %d = %d\n", i, sample[i]);
    }
    /* sample[5]、sample[-1] 都越界。讲义里打印出 379、0，只是某一次碰巧读到的内存，换一次运行就会变。 */
    return 0;
}
