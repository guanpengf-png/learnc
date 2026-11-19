#include <stdio.h>
int main()
{
  // goto是关键字,和标签名组合后就是goto语句,可以在代码执行的时候跳来跳去

  //   printf("1\n");
  //   goto three;
  //   printf("2\n");
  // three:
  //   printf("3\n");

  // 使用goto实现输出1到5
  int i = 1;
star:
  if (i <= 5)
  {
    printf("%d\n", i);
    i++;
    goto star;
  }

  return 0;
}