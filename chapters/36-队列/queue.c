/*
 * 章节：循环队列
 * 知识点：
 *   队列只允许在队尾插入，在队首删除。先进先出。
 *   如果入队只把 rear 往后移、出队只把 front 往后移，front 前面空出来的格子就浪费了。
 *   rear 走到数组末尾时，明明前面有空位，却不能再入队。这叫假溢出。
 *   循环队列把数组看成首尾相接的环。下标移动用 (下标 + 1) % capacity，走到末尾后回到 0。
 *   这一份另外用 length 记录元素个数，所以不会浪费一个格子。
 *     length == 0 是空。length == capacity 是满。
 *     front == rear 在这里不能单独说明空还是满，因为满的时候两个下标也会相遇。
 *   不用 length 的另一种写法会空出一个格子：front == rear 表示空，(rear + 1) % capacity == front 表示满。
 *   讲义的入队在满了以后直接 return。容量是 6 时，第 7 个元素 700 不会进入队列，长度仍然是 6。
 *   遍历要从 front 开始走 length 步，每一步都对 capacity 取余，不能从下标 0 一直打到末尾。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t length;
    size_t capacity;
    size_t front;
    size_t rear;
} Queue;

static bool init_queue(Queue *queue, size_t init_capacity)
{
    if (init_capacity == 0) {
        init_capacity = 1;
    }
    queue->data = malloc(init_capacity * sizeof *queue->data);
    queue->length = 0;
    queue->capacity = queue->data == NULL ? 0 : init_capacity;
    queue->front = 0;
    queue->rear = 0;
    return queue->data != NULL;
}

static size_t get_length(const Queue *queue)
{
    return queue->length;
}

static void destroy_queue(Queue *queue)
{
    free(queue->data);
    queue->data = NULL;
    queue->length = 0;
    queue->capacity = 0;
    queue->front = 0;
    queue->rear = 0;
}

static bool en_queue(Queue *queue, int element)
{
    if (queue->capacity == 0 || queue->length == queue->capacity) {
        return false;
    }
    queue->data[queue->rear] = element;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->length++;
    return true;
}

static bool de_queue(Queue *queue, int *removed)
{
    if (queue->length == 0) {
        return false;
    }
    *removed = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->length--;
    return true;
}

static void print_queue(const Queue *queue)
{
    if (queue->length == 0) {
        printf("(空)\n");
        return;
    }
    for (size_t i = 0; i < queue->length; i++) {
        size_t index = (queue->front + i) % queue->capacity;
        printf("%d ", queue->data[index]);
    }
    printf("\n");
}

int main(void)
{
    Queue queue;
    if (!init_queue(&queue, 6)) {
        printf("初始化失败\n");
        return 1;
    }

    en_queue(&queue, 100);
    en_queue(&queue, 200);
    en_queue(&queue, 300);
    en_queue(&queue, 400);
    en_queue(&queue, 500);
    en_queue(&queue, 600);
    printf("第 7 个 700: %s\n", en_queue(&queue, 700) ? "进入了" : "队列已满，没有进入");
    printf("长度 %zu\n", get_length(&queue));
    print_queue(&queue);

    int removed = 0;
    de_queue(&queue, &removed);
    de_queue(&queue, &removed);
    de_queue(&queue, &removed);
    printf("出队 3 个之后，最后出队的是 %d，长度 %zu\n", removed, get_length(&queue));
    print_queue(&queue);

    en_queue(&queue, 1000);
    en_queue(&queue, 2000);
    en_queue(&queue, 3000);
    printf("从空出来的前面再入队，长度 %zu\n", get_length(&queue));
    printf("front = %zu, rear = %zu\n", queue.front, queue.rear);
    print_queue(&queue);

    destroy_queue(&queue);
    printf("释放后长度 %zu\n", get_length(&queue));
    print_queue(&queue);
    return 0;
}
