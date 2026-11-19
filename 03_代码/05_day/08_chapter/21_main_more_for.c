#include <stdio.h>
int main()
{
  // 多重循环:多个循环可以嵌套,也就是所谓的嵌套循环,也就是循环中还有循环语句
  // while循环中可以有while循环/do-while循环/for循环
  // do-while循环中可以有while循环/do-while循环/for循环
  // for循环中可以有while循环/do-while循环/for循环
  int rows = 0, cols = 0;
  printf("请您输入行和列:\n");
  scanf("%d %d",&rows,&cols);
  for (int i = 0; i < rows; i++) // 控制的是行数
  {
    for (int j = 0; j < cols; j++) // 控制的是列数
      printf("[%d,%d]",i,j);
    printf("\n");
  }

  return 0;
}