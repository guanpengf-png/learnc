/*
 * 章节：break、continue、goto
 * 知识点：
 *   break 结束整个当前循环（或 switch），后面的轮次不再执行。
 *   continue 只结束本轮，然后进入下一轮判断。
 *   goto 跳到同一个函数里的标签。标签名要符合标识符规则。
 *   goto 会打乱从上到下的阅读顺序，正常写循环不要用它。
 *   它偶尔用来一次跳出好几层循环。这里只演示跳转本身。
 */
#include <stdio.h>

int main(void)
{
    printf("break:\n");
    for (int i = 0; i < 10; i++) {
        if (i == 3) {
            break;
        }
        printf("%d ", i);
    }
    printf("\n");

    printf("continue:\n");
    for (int i = 0; i <= 5; i++) {
        if (i == 3) {
            continue;
        }
        printf("%d ", i);
    }
    printf("\n");

    printf("1\n");
    goto three;
    printf("2\n");
three:
    printf("3\n");

    int n = 1;
start:
    if (n <= 5) {
        printf("%d\t", n);
        n++;
        goto start;
    }
    printf("\n");
    return 0;
}
