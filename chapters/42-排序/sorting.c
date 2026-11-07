/*
 * 章节：常见排序算法
 * 知识点：
 *   排序就是把一组数据按大小重新排列。衡量排序好坏看三件事：
 *     时间复杂度、额外空间、稳定性。
 *   稳定性：值相等的两个元素，排序前后相对顺序不变。比如按分数排序，同分的同学还保持原来的名单顺序。
 *
 *   冒泡排序：相邻两个比较，大的往后换。每一轮把当前最大值“冒”到末尾。
 *             最好 O(n)（已经有序且加了提前退出），平均/最坏 O(n^2)，稳定，原地。
 *   选择排序：每一轮从未排序部分挑最小的，和开头交换。
 *             永远 O(n^2)，不稳定（交换会打乱相等元素的顺序），原地。
 *   插入排序：像整理扑克牌，把新牌插到前面合适的位置。
 *             最好 O(n)（已经有序），平均/最坏 O(n^2)，稳定，原地。
 *             数据量小或接近有序时非常快，很多库在小区间会退回插入排序。
 *   归并排序：把数组对半切开，分别排好，再合并两个有序数组。
 *             永远 O(n log n)，稳定，但需要 O(n) 额外空间。
 *   快速排序：选一个基准，把比它小的放左边、大的放右边，再对左右两边递归。
 *             平均 O(n log n)，最坏 O(n^2)（比如已有序且总选最后一个当基准），不稳定，原地。
 *             实际工程里会随机选基准或三数取中来避免最坏情况。
 *   堆排序：  先建大根堆，再反复把堆顶换到末尾并缩小堆。
 *             永远 O(n log n)，不稳定，原地。
 *
 *   应用：数据库排序、排行榜、日志分析、去重前的整理；C 标准库的 qsort 就是快速排序的工程版。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void print_array(const char *label, const int *a, size_t n)
{
    printf("%s", label);
    for (size_t i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

static bool is_sorted(const int *a, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            return false;
        }
    }
    return true;
}

/* ---------------- 冒泡排序 ---------------- */
static void bubble_sort(int *a, size_t n)
{
    for (size_t end = n; end > 1; end--) {
        bool swapped = false;
        for (size_t i = 1; i < end; i++) {
            if (a[i - 1] > a[i]) {
                swap_int(&a[i - 1], &a[i]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;          /* 这一轮没交换，说明已经有序 */
        }
    }
}

/* ---------------- 选择排序 ---------------- */
static void selection_sort(int *a, size_t n)
{
    for (size_t i = 0; i + 1 < n; i++) {
        size_t min_index = i;
        for (size_t j = i + 1; j < n; j++) {
            if (a[j] < a[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            swap_int(&a[i], &a[min_index]);
        }
    }
}

/* ---------------- 插入排序 ---------------- */
static void insertion_sort(int *a, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        int key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = key;
    }
}

/* ---------------- 归并排序 ---------------- */
static void merge_ranges(int *a, int *temp, size_t left, size_t mid, size_t right)
{
    size_t i = left;
    size_t j = mid;
    size_t k = left;

    while (i < mid && j < right) {
        /* 用 <= 才能保持稳定：相等时先取左边的 */
        temp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    }
    while (i < mid) {
        temp[k++] = a[i++];
    }
    while (j < right) {
        temp[k++] = a[j++];
    }

    for (size_t x = left; x < right; x++) {
        a[x] = temp[x];
    }
}

static void merge_sort_range(int *a, int *temp, size_t left, size_t right)
{
    if (right - left < 2) {
        return;
    }
    size_t mid = left + (right - left) / 2;
    merge_sort_range(a, temp, left, mid);
    merge_sort_range(a, temp, mid, right);
    merge_ranges(a, temp, left, mid, right);
}

static bool merge_sort(int *a, size_t n)
{
    int *temp = malloc(n * sizeof *temp);
    if (temp == NULL) {
        return false;
    }
    merge_sort_range(a, temp, 0, n);
    free(temp);
    return true;
}

/* ---------------- 快速排序 ---------------- */
/* 以 a[high] 为基准，把小于它的放左边，返回基准最后的位置 */
static size_t partition(int *a, size_t low, size_t high)
{
    int pivot = a[high];
    size_t i = low;

    for (size_t j = low; j < high; j++) {
        if (a[j] < pivot) {
            swap_int(&a[i], &a[j]);
            i++;
        }
    }
    swap_int(&a[i], &a[high]);
    return i;
}

static void quick_sort_range(int *a, size_t low, size_t high)
{
    if (low >= high) {
        return;
    }
    size_t pivot_index = partition(a, low, high);

    if (pivot_index > low) {
        quick_sort_range(a, low, pivot_index - 1);
    }
    quick_sort_range(a, pivot_index + 1, high);
}

static void quick_sort(int *a, size_t n)
{
    if (n > 1) {
        quick_sort_range(a, 0, n - 1);
    }
}

/* ---------------- 堆排序 ---------------- */
static void sift_down(int *a, size_t size, size_t i)
{
    for (;;) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t largest = i;

        if (left < size && a[left] > a[largest]) {
            largest = left;
        }
        if (right < size && a[right] > a[largest]) {
            largest = right;
        }
        if (largest == i) {
            break;
        }
        swap_int(&a[i], &a[largest]);
        i = largest;
    }
}

static void heap_sort(int *a, size_t n)
{
    /* 1. 建大根堆 */
    for (size_t i = n / 2; i > 0; i--) {
        sift_down(a, n, i - 1);
    }
    /* 2. 每次把堆顶换到末尾，堆缩小，再下沉 */
    for (size_t end = n; end > 1; end--) {
        swap_int(&a[0], &a[end - 1]);
        sift_down(a, end - 1, 0);
    }
}

int main(void)
{
    int original[] = {5, 2, 9, 1, 5, 6, 3, 8, 7, 4};
    const size_t n = sizeof original / sizeof original[0];

    print_array("原始数组: ", original, n);
    printf("\n");

    int a[10];

    /* 每个排序都用一份新的拷贝，互不影响 */
    for (size_t i = 0; i < n; i++) a[i] = original[i];
    bubble_sort(a, n);
    print_array("冒泡排序:     ", a, n);

    for (size_t i = 0; i < n; i++) a[i] = original[i];
    selection_sort(a, n);
    print_array("选择排序:     ", a, n);

    for (size_t i = 0; i < n; i++) a[i] = original[i];
    insertion_sort(a, n);
    print_array("插入排序:     ", a, n);

    for (size_t i = 0; i < n; i++) a[i] = original[i];
    if (merge_sort(a, n)) {
        print_array("归并排序:     ", a, n);
    }

    for (size_t i = 0; i < n; i++) a[i] = original[i];
    quick_sort(a, n);
    print_array("快速排序:     ", a, n);

    for (size_t i = 0; i < n; i++) a[i] = original[i];
    heap_sort(a, n);
    print_array("堆排序:       ", a, n);

    printf("\n检查结果: %s\n", is_sorted(a, n) ? "全部有序" : "有错误");

    printf("\n复杂度与稳定性:\n");
    printf("  冒泡/选择/插入: O(n^2)，插入和冒泡最好 O(n)\n");
    printf("  归并: O(n log n)，需要 O(n) 额外空间，稳定\n");
    printf("  快速/堆: O(n log n)，原地，不稳定\n");
    return 0;
}
