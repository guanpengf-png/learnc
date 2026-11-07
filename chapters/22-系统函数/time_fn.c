/*
 * 章节：time.h 里的时间函数
 * 知识点：
 *   time_t 用来保存时间戳：从 1970-01-01 00:00:00 UTC 到现在的秒数。
 *   time(&变量) 把当前时间戳写进变量。
 *   ctime(&时间戳) 转成当地时间的字符串，末尾自带换行。
 *   difftime(结束, 开始) 返回相差的秒数，类型是 double，分辨率通常是整秒。
 *   很短的循环用 difftime 经常得到 0。要看更细的耗时，用 clock()，单位换成秒要除以 CLOCKS_PER_SEC。
 */
#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t current;
    time(&current);
    printf("当前时间戳: %lld\n", (long long)current);
    printf("当前时间字符串: %s", ctime(&current));

    time_t start;
    time_t end;
    time(&start);
    volatile long sum = 0;
    for (long i = 0; i < 2000000L; i++) {
        sum += i;
    }
    time(&end);
    printf("difftime = %.0f 秒, sum = %ld\n", difftime(end, start), sum);

    clock_t t0 = clock();
    volatile long other = 0;
    for (long i = 0; i < 2000000L; i++) {
        other += i;
    }
    clock_t t1 = clock();
    printf("clock 测得 %.6f 秒\n", (double)(t1 - t0) / CLOCKS_PER_SEC);
    return 0;
}
