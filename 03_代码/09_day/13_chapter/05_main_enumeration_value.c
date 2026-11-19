#include <stdio.h>
// enum SEASON
// {
//   spring,
//   summer=100,
//   autumn,
//   winter
// };
enum SEASON
{
  spring=101,
  summer,
  autumn,
  winter
};
int main()
{
  // 声明枚举类型的变量,赋值并输出
  // enum SEASON s = summer;
  // printf("s=%d\n",s);

  // 枚举类型的元素值默认是从0开始,本质是整型,所以,可以遍历
  for (enum SEASON s = spring; s <= winter; s++)
  {
    printf("%d\n", s);
  }

  return 0;
}