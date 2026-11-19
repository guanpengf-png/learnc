#include <stdio.h>
int main()
{
  // for循环的几种特殊写法:

  // 写法1:
  {
    for (int i = 0; i < 3; i++)
      printf("%d\n", i);
  }
  // 写法2:计数器的变量的声明可以放在外部
  {
    int i;
    for (i = 0; i < 3; i++)
      printf("%d\n", i);
    printf("最终i的值是:%d\n", i);
  }
  // 写法3:省略第一个表达式,放在外部
  {
    int i = 0;
    for (; i < 3; i++)
      printf("%d\n", i);
    printf("最终i的值是:%d\n", i);
  }
  // 写法4:第三个表达式可以放在大括号中书写
  {
    int i = 0;
    for (; i < 3;)
    {
      printf("%d\n", i);
      i++;
    }
    printf("最终i的值是:%d\n", i);
  }
  // 写法5:表达式2也可以省略
  {
    // 下面的代码是死循环
    // for (;;)
    //   printf("测试\n");
  }
  // 写法6:每个表达式的位置可以有多个式子组成
  for (int x = 0, y = 0, z = 0; x < 3 && y < 4 && z < 5; x++, y++, z++)
    printf("x=%d,y=%d,z=%d\n", x, y, z);


  

  return 0;
}