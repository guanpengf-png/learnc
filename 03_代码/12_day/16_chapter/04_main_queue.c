#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// 定义队列的结构体类型
typedef struct
{
  int *data;       // 指针(指向队列所需内存空间的内存地址)
  size_t capacity; // 容量
  size_t length;   // 长度
  size_t rear;     // 添加数据后的移动索引标识
  size_t front;    // 待删除数据的移动索引标识
} Queue;
// 初始化队列的函数
void init_queue(Queue *queue, size_t init_capacity)
{
  // 根据容量分配内存,并设置其指针的指向
  queue->data = (int *)malloc(sizeof(int) * init_capacity);
  queue->capacity = init_capacity; // 初始化容量
  queue->length = 0;               // 初始化长度
  queue->rear = 0;                 // 初始化添加数据后的移动索引标识
  queue->front = 0;                // 初始化待删除数据的移动索引标识
}
// 释放队列的函数
void destroy_queue(Queue *queue)
{
  free(queue->data);   // 释放
  queue->data = NULL;  // 重置为空指针
  queue->capacity = 0; // 重置容量
  queue->length = 0;   // 重置长度
  queue->rear = 0;     // 重置添加数据后的移动索引标识
  queue->front = 0;    // 重置待删除数据的移动索引标识
}
// 获取长度的函数
size_t get_length(Queue *queue)
{
  return queue->length;
}
// 入队
void en_queue(Queue *queue, int element)
{
  if (queue->length == queue->capacity) // 容量满了就不再添加数据
    return;
  queue->data[queue->rear] = element;                // 添加数据
  queue->rear = (queue->rear + 1) % queue->capacity; // 向后移动
  queue->length++;                                   // 更新长度加1
}
// 出队
bool out_queue(Queue *queue, int *del_element)
{
  if (queue->length == 0)
    return false;                                      // 没有数据了
  *del_element = queue->data[queue->front];            // 先保存要删除的数据
  queue->front = (queue->front + 1) % queue->capacity; // 向后移动
  queue->length--;                                     // 更新长度减1
  return true;
}
// 遍历队列
void printf_queue(Queue *queue)
{
  if (queue->length == 0)
    return;
  for (size_t i = 0; i < queue->length; i++)
  {
    // 每次遍历都要重新计算待删除数据的移动索引标识的位置
    size_t index = (queue->front + i) % queue->capacity;
    printf("%d ", queue->data[index]);
  }
  printf("\n");
}
int main()
{
  Queue q;
  init_queue(&q, 6); // 初始化队列
  en_queue(&q, 100);
  en_queue(&q, 200);
  en_queue(&q, 300);
  en_queue(&q, 400);
  en_queue(&q, 500);
  en_queue(&q, 600);
  en_queue(&q, 700);
  printf("长度:%zu\n", get_length(&q));
  printf_queue(&q); // 遍历
  int del_element = 0;
  out_queue(&q, &del_element); // 出队
  printf("长度:%zu\n", get_length(&q));
  printf_queue(&q);            // 遍历
  out_queue(&q, &del_element); // 出队
  printf("长度:%zu\n", get_length(&q));
  printf_queue(&q);            // 遍历
  out_queue(&q, &del_element); // 出队
  printf("长度:%zu\n", get_length(&q));
  printf_queue(&q); // 遍历
  en_queue(&q, 700);
  en_queue(&q, 800);
  printf("长度:%zu\n", get_length(&q));
  printf_queue(&q);  // 遍历
  destroy_queue(&q); // 释放了
  printf("长度:%zu\n", get_length(&q));
  printf_queue(&q); // 遍历

  return 0;
}