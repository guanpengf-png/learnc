#include<stdio.h>
enum GENDER
{
    male,
    female
};
enum GENDER g;
enum WEATHER{
    spring,
    summer,
    antumn,
    winter
}s1,s2,s3,s4;

int main(int argc, char const *argv[])
{
    // 1. 枚举本质是整数，默认从 0 开始递增
    printf("male=%d, female=%d\n", male, female);
    printf("spring=%d, summer=%d, antumn=%d, winter=%d\n",
           spring, summer, antumn, winter);

    // 2. 用枚举变量保存取值并打印
    s1 = spring;
    s2 = winter;
    printf("s1=%d, s2=%d\n", s1, s2);

    g = female;
    printf("g=%d, g==female? %s\n", g, g == female ? "yes" : "no");

    // 3. 配合 switch 使用，可读性更好
    switch (s1) {
        case spring: printf("现在是春天\n"); break;
        case summer: printf("现在是夏天\n"); break;
        case antumn: printf("现在是秋天\n"); break;
        case winter: printf("现在是冬天\n"); break;
    }

    // 4. 枚举变量的尺寸（通常和 int 一样）
    printf("sizeof(enum WEATHER)=%zu\n", sizeof(enum WEATHER));

    return 0;
}
