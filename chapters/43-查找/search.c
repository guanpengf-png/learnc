/*
 * 章节：查找算法
 * 知识点：
 *   线性查找（顺序查找）：从头到尾一个一个比。
 *     数组不需要有序，O(n)。数据很少或者只查一次时够用。
 *   二分查找：每次拿中间元素比较，比它小就只看左半边，比它大就只看右半边。
 *     前提：数组必须已经有序。每次排除一半，O(log n)。
 *     10 亿个数据，二分最多比较 30 次左右，线性最坏要 10 亿次。
 *   中间下标写成 low + (high - low) / 2，而不是 (low + high) / 2。
 *     后者在 low 和 high 都很大时可能整数溢出，变成负数下标。
 *   二分查找还有两个常见变体：
 *     lower_bound：第一个 >= key 的位置
 *     upper_bound：第一个 > key 的位置
 *     它们常用于“插入位置”“统计个数”“去重”等问题。
 *   应用：字典查词、数据库索引、版本回退（git bisect）、猜数字、游戏里按等级查配置表。
 */
#include <stdbool.h>
#include <stdio.h>

/* 线性查找：返回下标，找不到返回 -1；steps 记录比较次数 */
static int linear_search(const int *a, size_t n, int key, size_t *steps)
{
    *steps = 0;
    for (size_t i = 0; i < n; i++) {
        (*steps)++;
        if (a[i] == key) {
            return (int)i;
        }
    }
    return -1;
}

/* 迭代版二分查找 */
static int binary_search(const int *a, size_t n, int key, size_t *steps)
{
    size_t low = 0;
    size_t high = n;      /* 区间是 [low, high) */

    *steps = 0;

    while (low < high) {
        size_t mid = low + (high - low) / 2;   /* 防止 low + high 溢出 */
        (*steps)++;

        if (a[mid] == key) {
            return (int)mid;
        }
        if (a[mid] < key) {
            low = mid + 1;      /* 答案在右半边 */
        } else {
            high = mid;         /* 答案在左半边 */
        }
    }
    return -1;
}

/* 递归版二分查找 */
static int binary_search_recursive(const int *a, size_t low, size_t high, int key)
{
    if (low >= high) {
        return -1;
    }
    size_t mid = low + (high - low) / 2;
    if (a[mid] == key) {
        return (int)mid;
    }
    if (a[mid] < key) {
        return binary_search_recursive(a, mid + 1, high, key);
    }
    return binary_search_recursive(a, low, mid, key);
}

/* 第一个 >= key 的下标；都比 key 小就返回 n */
static size_t lower_bound(const int *a, size_t n, int key)
{
    size_t low = 0;
    size_t high = n;

    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (a[mid] < key) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

int main(void)
{
    int sorted[] = {1, 3, 5, 7, 9, 11, 13, 15};
    size_t n = sizeof sorted / sizeof sorted[0];

    printf("有序数组: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n\n");

    size_t steps = 0;

    int index = linear_search(sorted, n, 7, &steps);
    printf("线性查找 7: 下标 %d，比较 %zu 次\n", index, steps);

    index = binary_search(sorted, n, 7, &steps);
    printf("二分查找 7: 下标 %d，比较 %zu 次\n", index, steps);

    index = binary_search(sorted, n, 8, &steps);
    printf("二分查找 8: 下标 %d（不存在），比较 %zu 次\n", index, steps);

    index = binary_search_recursive(sorted, 0, n, 13);
    printf("递归二分查找 13: 下标 %d\n", index);

    size_t pos = lower_bound(sorted, n, 8);
    printf("lower_bound(8) = %zu（第一个 >= 8 的位置，可以插在这里）\n", pos);

    pos = lower_bound(sorted, n, 100);
    printf("lower_bound(100) = %zu（等于数组长度，说明都在它左边）\n", pos);

    printf("\n数据量越大，差距越明显:\n");
    printf("  线性查找: O(n)，100 万个数据最坏要看 100 万次\n");
    printf("  二分查找: O(log n)，100 万个数据最多比较 20 次\n");
    return 0;
}
