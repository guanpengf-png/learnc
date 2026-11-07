/*
 * 章节：顺序栈
 * 知识点：
 *   栈是只允许在同一端插入和删除的线性表。这一端叫栈顶，另一端叫栈底。
 *   后进先出，也说先进后出。函数调用就是栈：最先调用的函数最后返回。
 *   这一份用动态数组实现，也就是顺序栈。栈底放在下标 0，栈顶下标是 length - 1。
 *   空栈的 length 是 0。这和“栈顶下标初始为 -1”是同一件事，只是这里用个数而不是用一个可能为负的下标。
 *   入栈写在 data[length]，然后 length 加 1。满了就把容量扩成两倍。
 *   出栈先读 data[length - 1]，再把 length 减 1。元素可以先留在数组里，逻辑上已经不在栈中。
 *   遍历从下标 0 打到最后，是从栈底看到栈顶，不是出栈顺序。
 *   讲义的 realloc 同样不能在失败时覆盖原来的指针。空栈出栈要直接失败，不能去算 length - 1。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t length;
    size_t capacity;
} Stack;

static bool resize_stack(Stack *stack, size_t new_capacity)
{
    if (new_capacity == 0) {
        new_capacity = 1;
    }
    int *grown = realloc(stack->data, new_capacity * sizeof *grown);
    if (grown == NULL) {
        return false;
    }
    stack->data = grown;
    stack->capacity = new_capacity;
    return true;
}

static bool init_stack(Stack *stack, size_t init_capacity)
{
    if (init_capacity == 0) {
        init_capacity = 1;
    }
    stack->data = malloc(init_capacity * sizeof *stack->data);
    stack->length = 0;
    stack->capacity = stack->data == NULL ? 0 : init_capacity;
    return stack->data != NULL;
}

static void destroy_stack(Stack *stack)
{
    free(stack->data);
    stack->data = NULL;
    stack->length = 0;
    stack->capacity = 0;
}

static size_t get_length(const Stack *stack)
{
    return stack->length;
}

static bool push_stack(Stack *stack, int element)
{
    if (stack->length == stack->capacity) {
        if (!resize_stack(stack, stack->capacity * 2)) {
            return false;
        }
    }
    stack->data[stack->length] = element;
    stack->length++;
    return true;
}

static bool pop_stack(Stack *stack, int *popped)
{
    if (stack->length == 0) {
        return false;
    }
    *popped = stack->data[stack->length - 1];
    stack->length--;
    return true;
}

static void print_stack(const Stack *stack)
{
    if (stack->length == 0) {
        printf("(空)\n");
        return;
    }
    printf("从栈底到栈顶: ");
    for (size_t i = 0; i < stack->length; i++) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}

int main(void)
{
    Stack stack;
    if (!init_stack(&stack, 3)) {
        printf("初始化失败\n");
        return 1;
    }

    push_stack(&stack, 10);
    push_stack(&stack, 20);
    push_stack(&stack, 30);
    printf("入栈 3 个，长度 %zu，容量 %zu\n", get_length(&stack), stack.capacity);
    push_stack(&stack, 40);
    printf("第 4 个把容量扩成 %zu\n", stack.capacity);
    print_stack(&stack);

    int popped = 0;
    pop_stack(&stack, &popped);
    printf("出栈 %d\n", popped);
    pop_stack(&stack, &popped);
    printf("出栈 %d\n", popped);
    printf("长度 %zu\n", get_length(&stack));
    print_stack(&stack);

    destroy_stack(&stack);
    printf("释放后出栈: %s\n", pop_stack(&stack, &popped) ? "成功" : "失败");
    return 0;
}
