/*
 * 章节：if / else / else if
 * 知识点：
 *   条件为非 0 才进入 if。0 是假。
 *   单向 if：只有成立才做额外的事，后面的语句总会执行。
 *   if...else：二选一，两条路一定走一条。
 *   else if：从上往下找第一个成立的条件，后面的不再判断。
 *   区间判断要先写更严格的条件。成绩先判断 >= 90，再 >= 80，再 >= 60。
 */
#include <stdio.h>

static void check_minor(int age)
{
    printf("年龄 %d: ", age);
    if (age <= 18) {
        printf("不能看电影 \n");
    }
    printf("程序继续\n");
}

static void check_responsibility(int age)
{
    printf("年龄 %d: ", age);
    if (age < 18) {
        printf("回家写作业吧,孩子\n");
    } else {
        printf("成年了,要负责哦\n");
    }
}

static void reward(int score)
{
    printf("成绩 %d: ", score);
    if (score >= 90) {
        printf("奖励一个华为手机\n");
    } else if (score >= 80) {
        printf("奖励一个平板\n");
    } else if (score >= 60) {
        printf("奖励一个肉夹馍\n");
    } else {
        printf("没有奖励\n");
    }
}

int main(void)
{
    check_minor(16);
    check_minor(20);
    check_responsibility(16);
    check_responsibility(20);
    reward(95);
    reward(85);
    reward(70);
    reward(50);
    return 0;
}
