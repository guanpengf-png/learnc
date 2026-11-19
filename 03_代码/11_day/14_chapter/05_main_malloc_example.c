#include <stdio.h>
#include <stdlib.h>
// 声明函数原型
void change_scores(double *, int);
int main()
{
  // 动态创建数组，输入5个学生的成绩，再定义一个函数检测成绩低于60分的，输出不合格的成绩。
  int len = 5;
  // 动态分配内存
  double *scores = (double *)malloc(sizeof(double) * len);
  if (scores == NULL)
  {
    printf("内存分配失败\n");
    return 0;
  }
  // 遍历的方式录入数据
  for (int i = 0; i < len; i++)
  {
    printf("请您输入第%d个学员的成绩:\n",i+1);
    scanf("%lf", scores + i);
  }
  change_scores(scores,len); // 检测成绩
  free(scores); // 释放内存
  scores = NULL; // 重置为空指针

  return 0;
}
// 函数定义(函数实现)
void change_scores(double *ptr, int len)
{
  for (int i = 0; i < len; i++)
    if (ptr[i] < 60)
      printf("%.2lf====继续加油\n", ptr[i]);
}