/*
 * 章节：堆与优先队列
 * 知识点：
 *   堆是一棵完全二叉树，而且用数组存，不需要指针。完全二叉树按层填满，所以下标关系是固定的。
 *   大根堆：每个结点的值都不小于它的孩子，所以最大值一定在下标 0（堆顶）。
 *   小根堆相反，最小值在堆顶。
 *   下标关系（数组从 0 开始）：
 *     父结点 = (i - 1) / 2
 *     左孩子 = 2 * i + 1
 *     右孩子 = 2 * i + 2
 *   入堆：先放到数组末尾，再和父结点比，比父大就交换，一路往上冒（上浮），O(log n)。
 *   出堆：先记下堆顶，把最后一个元素搬到堆顶，再和两个孩子里较大的那个比，
 *         比孩子小就交换，一路往下沉（下沉），O(log n)。
 *   建堆：从最后一个非叶子结点（n/2 - 1）开始依次下沉。
 *         乍看是 O(n log n)，但数学上求和后是 O(n)：越靠下的结点越多，能下沉的距离越短。
 *   堆只保证“堆顶是最值”，并不保证整体有序。要整体有序得反复出堆（堆排序）。
 *   应用：优先队列（任务调度、事件模拟）、Top K、Dijkstra 优化、定时器、哈夫曼编码。
 */
#include <stdbool.h>
#include <stdio.h>

#define HEAP_CAPACITY 64

typedef struct {
    int data[HEAP_CAPACITY];
    size_t size;
} MaxHeap;

static void init_heap(MaxHeap *heap)
{
    heap->size = 0;
}

static void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* 下标 i 往下沉，直到比孩子都大为止 */
static void sift_down(MaxHeap *heap, size_t i)
{
    for (;;) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t largest = i;

        if (left < heap->size && heap->data[left] > heap->data[largest]) {
            largest = left;
        }
        if (right < heap->size && heap->data[right] > heap->data[largest]) {
            largest = right;
        }
        if (largest == i) {
            break;
        }
        swap_int(&heap->data[i], &heap->data[largest]);
        i = largest;
    }
}

static bool heap_push(MaxHeap *heap, int value)
{
    if (heap->size == HEAP_CAPACITY) {
        return false;
    }

    size_t i = heap->size;
    heap->data[i] = value;
    heap->size++;

    /* 上浮：比父结点大就交换 */
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (heap->data[parent] >= heap->data[i]) {
            break;
        }
        swap_int(&heap->data[parent], &heap->data[i]);
        i = parent;
    }
    return true;
}

static bool heap_pop(MaxHeap *heap, int *value)
{
    if (heap->size == 0) {
        return false;
    }

    *value = heap->data[0];
    heap->size--;

    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];   /* 最后一个元素搬上去 */
        sift_down(heap, 0);
    }
    return true;
}

static bool heap_peek(const MaxHeap *heap, int *value)
{
    if (heap->size == 0) {
        return false;
    }
    *value = heap->data[0];
    return true;
}

/* 用现成的数组建堆，O(n) */
static void heap_build(MaxHeap *heap, const int *values, size_t count)
{
    if (count > HEAP_CAPACITY) {
        count = HEAP_CAPACITY;
    }
    for (size_t i = 0; i < count; i++) {
        heap->data[i] = values[i];
    }
    heap->size = count;

    for (size_t i = count / 2; i > 0; i--) {
        sift_down(heap, i - 1);
    }
}

static void print_heap(const MaxHeap *heap)
{
    printf("数组: ");
    for (size_t i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}

int main(void)
{
    /* 先演示建堆：同一组数，建完堆后下标 0 一定是最大值 */
    int values[] = {3, 1, 6, 5, 2, 4};
    MaxHeap heap;
    init_heap(&heap);
    heap_build(&heap, values, sizeof values / sizeof values[0]);

    printf("用 3 1 6 5 2 4 建大根堆\n");
    print_heap(&heap);

    int top = 0;
    heap_peek(&heap, &top);
    printf("堆顶（最大值）: %d\n\n", top);

    /* 一直出堆，得到的是从大到小 */
    printf("反复出堆: ");
    while (heap_pop(&heap, &top)) {
        printf("%d ", top);
    }
    printf("\n\n");

    /* 再演示入堆：优先队列的用法 */
    init_heap(&heap);
    int tasks[] = {15, 42, 8, 27, 33};
    for (size_t i = 0; i < sizeof tasks / sizeof tasks[0]; i++) {
        heap_push(&heap, tasks[i]);
    }
    printf("依次加入任务优先级 15 42 8 27 33\n");
    print_heap(&heap);

    heap_push(&heap, 50);
    printf("加入优先级 50 后: ");
    print_heap(&heap);

    heap_pop(&heap, &top);
    printf("取出最高优先级 %d，剩下: ", top);
    print_heap(&heap);

    return 0;
}
