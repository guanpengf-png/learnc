/*
 * 章节：可变长动态数组
 * 知识点：
 *   普通数组长度固定。按下标读写是 O(1)，因为地址可以算出来。
 *   在中间插入或删除要保持后面的元素仍然连续，所以要搬动一批元素。
 *   插入平均大约搬动 n/2 个，删除平均大约搬动 (n-1)/2 个。
 *   动态数组用一块堆内存保存元素，另外记住两个数：
 *     length 是现在真正有几个元素。
 *     capacity 是这块内存最多能放几个。length 不能大于 capacity。
 *   放满以后，用 realloc 申请一块更大的空间，通常扩成原来的两倍，再把旧元素留在新空间里。
 *   讲义里有三处不能照抄：
 *     realloc 失败时不能把返回的 NULL 直接写进 data，否则旧地址丢失。
 *     删除下标要满足 index < length。写成 index > length 时，index == length 会读到最后一个元素的后面。
 *     数组为空时，length - 1 用在 size_t 上会绕成一个很大的数，删尾之前要先判断空。
 *     destroy 只把 capacity 清零是不够的，length 也要清零。
 *   讲义把 free 的作用写成避免内存溢出。free 防止的是内存泄漏。写过界才叫溢出。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t length;
    size_t capacity;
} DynamicArray;

static bool resize_array(DynamicArray *array, size_t new_capacity)
{
    if (new_capacity == 0) {
        new_capacity = 1;
    }
    int *grown = realloc(array->data, new_capacity * sizeof *grown);
    if (grown == NULL) {
        return false;
    }
    array->data = grown;
    array->capacity = new_capacity;
    return true;
}

static bool init_array(DynamicArray *array, size_t init_capacity)
{
    if (init_capacity == 0) {
        init_capacity = 1;
    }
    array->data = malloc(init_capacity * sizeof *array->data);
    array->length = 0;
    array->capacity = array->data == NULL ? 0 : init_capacity;
    return array->data != NULL;
}

static void destroy_array(DynamicArray *array)
{
    free(array->data);
    array->data = NULL;
    array->length = 0;
    array->capacity = 0;
}

static size_t get_length(const DynamicArray *array)
{
    return array->length;
}

static bool insert_at(DynamicArray *array, size_t index, int element)
{
    if (index > array->length) {
        return false;
    }
    if (array->length == array->capacity) {
        if (!resize_array(array, array->capacity * 2)) {
            return false;
        }
    }
    for (size_t i = array->length; i > index; i--) {
        array->data[i] = array->data[i - 1];
    }
    array->data[index] = element;
    array->length++;
    return true;
}

static bool insert_end(DynamicArray *array, int element)
{
    return insert_at(array, array->length, element);
}

static bool delete_at(DynamicArray *array, size_t index, int *deleted)
{
    if (index >= array->length) {
        return false;
    }
    *deleted = array->data[index];
    for (size_t i = index; i + 1 < array->length; i++) {
        array->data[i] = array->data[i + 1];
    }
    array->length--;
    return true;
}

static bool delete_end(DynamicArray *array, int *deleted)
{
    if (array->length == 0) {
        return false;
    }
    return delete_at(array, array->length - 1, deleted);
}

static void print_array(const DynamicArray *array)
{
    if (array->length == 0) {
        printf("(空)\n");
        return;
    }
    for (size_t i = 0; i < array->length; i++) {
        printf("%d ", array->data[i]);
    }
    printf("\n");
}

int main(void)
{
    DynamicArray array;
    if (!init_array(&array, 5)) {
        printf("初始化失败\n");
        return 1;
    }

    insert_at(&array, 0, 30);
    insert_at(&array, 0, 20);
    insert_at(&array, 0, 10);
    printf("三次头部插入后，长度 %zu，容量 %zu\n", get_length(&array), array.capacity);
    insert_end(&array, 40);
    insert_end(&array, 50);
    printf("再追加 40 和 50，长度 %zu，容量 %zu\n", get_length(&array), array.capacity);
    print_array(&array);

    int deleted = 0;
    delete_at(&array, 3, &deleted);
    printf("删除下标 3，删掉的是 %d\n", deleted);
    delete_end(&array, &deleted);
    printf("删除尾部，删掉的是 %d\n", deleted);
    printf("长度 %zu\n", get_length(&array));
    print_array(&array);

    insert_end(&array, 40);
    insert_end(&array, 50);
    insert_end(&array, 60);
    printf("再放满并多放一个之后，长度 %zu，容量 %zu\n", get_length(&array), array.capacity);
    print_array(&array);

    destroy_array(&array);
    printf("释放后再删尾部: %s\n", delete_end(&array, &deleted) ? "成功" : "失败");
    return 0;
}
