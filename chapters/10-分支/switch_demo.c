/*
 * 章节：switch
 * 知识点：
 *   switch 后面的表达式必须是整数或枚举，不能是浮点，也不能写区间。
 *   case 后面必须是常量，不能是变量。
 *   匹配到某个 case 后，如果没有 break，会继续执行下面的 case，这叫穿透。
 *   月份这个例子故意利用穿透：1、3、5、7、8、10、12 共用同一段输出。
 *   default 处理所有没匹配上的值，建议写上。
 *   判等用 switch 更清楚；比大小、看区间用 if...else if。
 */
#include <stdio.h>

static void weekday(char day)
{
    printf("输入 %c: ", day);
    switch (day) {
    case 'a':
        printf("星期一开飞机\n");
        break;
    case 'b':
        printf("星期二去吃饭\n");
        break;
    case 'c':
        printf("星期三去搬砖\n");
        break;
    case 'd':
        printf("星期四去练字\n");
        break;
    default:
        printf("输入有误\n");
        break;
    }
}

static void days_of_month(int month)
{
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        printf("%d 月有31天\n", month);
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        printf("%d 月有30天\n", month);
        break;
    case 2:
        printf("%d 月有28天或29天\n", month);
        break;
    default:
        printf("输入有误\n");
        break;
    }
}

int main(void)
{
    weekday('a');
    weekday('c');
    weekday('x');
    days_of_month(2);
    days_of_month(4);
    days_of_month(12);
    days_of_month(13);
    return 0;
}
