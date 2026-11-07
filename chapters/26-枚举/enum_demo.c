/*
 * 章节：枚举
 * 知识点：
 *   enum 给一组相关的整数常量起名字。不写值时从 0 开始，后面逐个加 1。
 *   某个成员写了值，它后面没写值的成员从这个值继续加 1。每个值在同一个枚举里要唯一。
 *   枚举变量最好只取这些名字。C 仍允许把别的整数赋给它，编译不一定报错，但那就失去了枚举的意义。
 *   值是连续整数时，可以用循环从第一个走到最后一个。中间断开的值不能靠 ++ 遍历。
 *   枚举适合作为 switch 的判断对象，case 后面直接写枚举常量的名字。
 */
#include <stdio.h>

int main(void)
{
    enum Week {
        MONDAY = 1,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };
    enum Week day = WEDNESDAY;
    printf("WEDNESDAY = %d\n", day);
    day = FRIDAY;
    printf("FRIDAY = %d\n", day);

    printf("一周:");
    for (int d = MONDAY; d <= SUNDAY; d++) {
        printf(" %d", d);
    }
    printf("\n");

    enum Season {
        SPRING = 1,
        SUMMER,
        AUTUMN,
        WINTER
    } season = AUTUMN;

    switch (season) {
    case SPRING:
        printf("春季\n");
        break;
    case SUMMER:
        printf("夏季\n");
        break;
    case AUTUMN:
        printf("秋季\n");
        break;
    case WINTER:
        printf("冬季\n");
        break;
    default:
        printf("没有这个季节\n");
        break;
    }
    return 0;
}
