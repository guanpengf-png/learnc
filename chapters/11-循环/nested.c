/*
 * 章节：多重循环
 * 知识点：
 *   一个循环放在另一个循环里面，就形成嵌套。
 *   外层走 1 次，内层会完整地走完自己的全部次数。
 *   外层次数 m、内层次数 n，内层循环体实际执行 m * n 次。
 *   内层变量每次外层迭代都要重新从 0 开始。for 的初始化正好做这件事。
 */
#include <stdio.h>

int main(void)
{
    int rows = 3;
    int cols = 4;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("[%d,%d]", i, j);
        }
        printf("\n");
    }
    return 0;
}
