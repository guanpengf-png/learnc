/*
 * 章节：二维数组应用，三个班的成绩
 * 知识点：
 *   行表示班，列表示这个班里的学生。
 *   每个班的总分在进入下一行之前要清零，否则会把上一个班的分数带进去。
 *   全校平均分的分母是 行数 * 列数，也就是所有学生人数。
 *   讲义用 scanf 从键盘读入。这里直接放好三组成绩，方便反复运行对照。
 *   键盘版把 scanf("%lf", &scores[i][j]); 放进两层循环即可。
 */
#include <stdio.h>

int main(void)
{
    double scores[3][5] = {
        {80, 90, 70, 60, 100},
        {70, 70, 70, 70, 70},
        {90, 80, 70, 60, 50}
    };
    int rows = (int)(sizeof scores / sizeof scores[0]);
    int cols = (int)(sizeof scores[0] / sizeof scores[0][0]);
    double all_sum = 0;

    for (int i = 0; i < rows; i++) {
        double class_sum = 0;
        for (int j = 0; j < cols; j++) {
            class_sum += scores[i][j];
        }
        all_sum += class_sum;
        printf("第 %d 班总分 %.2f, 平均分 %.2f\n", i + 1, class_sum, class_sum / cols);
    }
    printf("全部总分 %.2f, 总平均 %.2f\n", all_sum, all_sum / (rows * cols));
    return 0;
}
