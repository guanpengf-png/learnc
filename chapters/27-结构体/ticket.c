/*
 * 章节：结构体应用，景区门票
 * 知识点：
 *   把相关数据放进结构体，再把结构体的地址传给函数，函数就能改这个人的成员。
 *   讲义文字写的是“年龄大于 18 收费 20，否则免费”。示例代码写成了 >= 18。
 *   这里按文字规则：年龄要大于 18 才收费。正好 18 岁免费。
 *   讲义用循环读名字，名字是 n 就退出。strcmp 相等时返回 0，不相等时返回的不一定是 1。
 *   scanf 读字符数组时不要加 &，数组名已经是地址。名字数组要留住结束符，输入不能比数组更长。
 */
#include <stdio.h>
#include <string.h>

struct Visitor {
    char name[20];
    int age;
    double pay;
};

static void set_pay(struct Visitor *person)
{
    person->pay = person->age > 18 ? 20 : 0;
}

int main(void)
{
    struct Visitor people[] = {
        {"ann", 20, 0},
        {"bob", 18, 0},
        {"cici", 12, 0}
    };
    int count = (int)(sizeof people / sizeof people[0]);
    for (int i = 0; i < count; i++) {
        set_pay(&people[i]);
        printf("%s, %d 岁, 门票 %.2f\n", people[i].name, people[i].age, people[i].pay);
    }

    char stop[] = "n";
    if (strcmp(stop, "n") == 0) {
        printf("名字等于 n 时结束输入\n");
    }
    return 0;
}
