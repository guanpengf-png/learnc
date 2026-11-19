#include <stdio.h>
int main()
{
  int rows = 3, cols = 5;     // 声明行和列
  int scores[rows][cols];     // 声明二维数组
  double class_score = 0;     // 存储的是每个班级的总成绩
  double all_class_score = 0; // 存储所有班级的成绩
  // 遍历二维数组,提示用户录入成绩
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
    {
      printf("请您输入第%d个班级中第%d个学员的成绩:\n", i + 1, j + 1);
      scanf("%d", &scores[i][j]);
    }
  // 遍历二维数组计算相关的成绩数据

  for (int i = 0; i < rows; i++)
  {
    class_score = 0; // 重置上次存储的成绩为0
    for (int j = 0; j < cols; j++)
      class_score += scores[i][j];  // 计算当前班级的总成绩
    all_class_score += class_score; // 计算总成绩
    printf("第%d个班级的总成绩是:%.2lf,平均分是%.2lf\n", i + 1, class_score, class_score / cols);
  }
  printf("总成绩是:%.2lf,班级的平均分:%.2lf\n", all_class_score, all_class_score / rows);

  return 0;
}