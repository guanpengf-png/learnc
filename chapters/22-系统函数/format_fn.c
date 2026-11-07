/*
 * 章节：sprintf 和 sscanf
 * 知识点：
 *   sprintf(目的数组, 格式, ...) 和 printf 的格式一样，但结果写进数组，不打到屏幕上。目的数组要够长。
 *   sscanf(来源字符串, 格式, ...) 和 scanf 的格式一样，但数据来自字符串，不是键盘。
 *   sscanf 的返回值是成功转换的项数。格式文本必须和来源字符串逐字对上。
 */
#include <stdio.h>

int main(void)
{
    int age = 10;
    double score = 28.234;
    char name[] = "小甜甜";
    char output_string[100];

    sprintf(output_string, "我是%s,今年%d岁,成绩是:%.2f", name, age, score);
    printf("%s\n", output_string);

    char input_string[] = "小甜甜成绩:45,大甜甜成绩:55,甜甜成绩:60,还有小白";
    float score1;
    float score2;
    float score3;
    char tail[32];
    int matched = sscanf(input_string,
                          "小甜甜成绩:%f,大甜甜成绩:%f,甜甜成绩:%f,还有%s",
                          &score1, &score2, &score3, tail);
    printf("匹配到 %d 项: %.2f, %.2f, %.2f, %s\n", matched, score1, score2, score3, tail);
    return 0;
}
