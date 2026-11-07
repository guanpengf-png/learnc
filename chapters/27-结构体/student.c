/*
 * 章节：结构体
 * 知识点：
 *   数组里的元素必须是同一种类型。一个人的学号、姓名、年龄类型不同，要用 struct 捆在一起。
 *   结构体是类型，结构体变量才是按照这个类型造出来的一个对象。
 *   定义变量有三种位置：类型定义完再写变量；定义类型的同时写变量；甚至可以不给类型起名字。
 *   没起名字的结构体以后无法再声明同类型的变量，也没法作为函数参数的类型，能不用就不用。
 *   用 . 访问成员。
 *   char *name 可以直接用字符串字面量赋值。char name[50] 是数组，不能对名字用 = 赋一整串，初始化时可以，之后要用 strcpy。
 *   指针指向结构体时，(*p).成员 和 p->成员 是一回事。
 */
#include <stdio.h>
#include <string.h>

struct Student {
    int id;
    char name[50];
    char gender;
    int age;
};

static void print_student(const struct Student *p)
{
    printf("学号 %d, 姓名 %s, 性别 %c, 年龄 %d\n", p->id, p->name, p->gender, p->age);
}

int main(void)
{
    struct Student stu;
    stu.id = 110;
    strcpy(stu.name, "jack");
    stu.gender = 'm';
    stu.age = 18;
    print_student(&stu);

    struct Student tom = {119, "tom", 'm', 18};
    struct Student lucy = {911, "lucy", 'f', 16};
    print_student(&tom);
    printf("用点号: %s\n", lucy.name);
    printf("先解引用: %s\n", (*&lucy).name);
    return 0;
}
