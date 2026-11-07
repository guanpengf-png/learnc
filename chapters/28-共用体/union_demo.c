/*
 * 章节：共用体
 * 知识点：
 *   union 的成员共用同一块内存。同一时刻只有最后写入的那个成员有意义。
 *   结构体的大小大致是各成员大小相加，还要考虑对齐。
 *   共用体的大小等于最长的那个成员，再按对齐补齐。好处是省空间。
 *   用 . 访问成员。初始化时 {.成员 = 值} 可以指定写给谁。
 *   只写一个值、不写成员名时，赋给的是第一个成员。
 *   学生和老师可以放进同一个结构体：大家都有姓名、编号、性别、职业；
 *   分数和课程不会同时存在，所以这两项放进共用体。
 */
#include <stdio.h>

union Data {
    int a;
    char c;
    float f;
};

struct Person {
    char name[20];
    int num;
    char sex;
    char work;
    union {
        float score;
        char course[20];
    } sc;
};

int main(void)
{
    union Data d1;
    d1.c = 'A';
    printf("写入的字符: %c\n", d1.c);

    union Data d2 = {.f = 1.5f};
    printf("指定成员初始化: %.1f\n", d2.f);

    union Data d3 = {30};
    printf("初始化第一个成员: %d\n", d3.a);
    printf("sizeof(union Data) = %zu, sizeof(int) = %zu, sizeof(float) = %zu\n",
           sizeof(union Data), sizeof(int), sizeof(float));

    struct Person people[2] = {
        {"ann", 1, 'f', 's', {.score = 91.5f}},
        {"lee", 2, 'm', 't', {.course = "math"}}
    };
    printf("姓名\t编号\t性别\t职业\t分数或课程\n");
    for (int i = 0; i < 2; i++) {
        if (people[i].work == 's') {
            printf("%s\t%d\t%c\t%c\t%.1f\n",
                   people[i].name, people[i].num, people[i].sex,
                   people[i].work, people[i].sc.score);
        } else {
            printf("%s\t%d\t%c\t%c\t%s\n",
                   people[i].name, people[i].num, people[i].sex,
                   people[i].work, people[i].sc.course);
        }
    }
    printf("人的结构体 %zu 字节，里面的共用体 %zu 字节\n",
           sizeof(struct Person), sizeof people[0].sc);
    return 0;
}
