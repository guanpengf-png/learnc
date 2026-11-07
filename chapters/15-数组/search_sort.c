/*
 * 章节：顺序查找、二分查找、冒泡排序
 * 知识点：
 *   讲义这里只留了标题，没有例程。下面按这三个算法的标准写法补上。
 *   顺序查找：从头看到尾。找到返回下标，找不到返回 -1。数组不用有序。
 *   二分查找：每次丢掉一半。前提是数组已经从小到大排好。也是找到返回下标，否则返回 -1。
 *   冒泡排序：相邻两个比较，较大的往后换。外层每走一轮，末尾就多一个已经排好的元素。
 */
#include <stdio.h>

static int sequential_search(const int arr[], int len, int target)
{
    for (int i = 0; i < len; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

static int binary_search(const int arr[], int len, int target)
{
    int left = 0;
    int right = len - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

static void bubble_sort(int arr[], int len)
{
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

static void print_arr(const int arr[], int len)
{
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    int raw[] = {4, 7, 1, 9, 3, 7};
    int raw_len = (int)(sizeof raw / sizeof raw[0]);
    printf("顺序查找 9，下标 = %d\n", sequential_search(raw, raw_len, 9));
    printf("顺序查找 8，下标 = %d\n", sequential_search(raw, raw_len, 8));
    printf("顺序查找 7，第一次出现的下标 = %d\n", sequential_search(raw, raw_len, 7));

    int sorted[] = {1, 3, 4, 7, 7, 9};
    int sorted_len = (int)(sizeof sorted / sizeof sorted[0]);
    printf("二分查找 4，下标 = %d\n", binary_search(sorted, sorted_len, 4));
    printf("二分查找 2，下标 = %d\n", binary_search(sorted, sorted_len, 2));

    int messy[] = {5, 1, 4, 2, 8};
    bubble_sort(messy, (int)(sizeof messy / sizeof messy[0]));
    printf("冒泡排序后: ");
    print_arr(messy, (int)(sizeof messy / sizeof messy[0]));
    return 0;
}
