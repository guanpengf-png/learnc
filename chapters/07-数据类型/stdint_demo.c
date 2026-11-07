/*
 * 章节：精确宽度整数
 * 知识点：
 *   short、int、long 的宽度随平台变化。
 *   <stdint.h> 提供 int32_t 这类别名，保证正好 32 位，嵌入式和跨平台代码常用。
 *   它只是别名：int 是 32 位时指向 int，否则可能指向 long。
 *   PRId32 是打印 int32_t 的可移植占位符，定义在 <inttypes.h>。
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void)
{
    int32_t x32 = 1234567;
    printf("x32 = %" PRId32 "\n", x32);
    printf("x32变量中存储数据的字节长度为:%zu\n", sizeof(int32_t));
    printf("short=%zu int=%zu long=%zu long long=%zu\n",
       sizeof(short), sizeof(int), sizeof(long), sizeof(long long));

    return 0;
}
