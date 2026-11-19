#include <stdio.h>
int computed_peach(int day){
  if(day==10)
    return 1;
  return (computed_peach(day + 1) + 1) * 2;
}
int main()
{
  // 有一堆桃子，猴子第一天吃了其中的一半，并多吃一个。以后每天猴子都吃其中的一半，然后再多吃一个。当到第十天时，想再吃时（注意：此时还没吃），发现只有1个桃子了。问：最初共多少个桃子？

  int peach_num = computed_peach(1);
  printf("结果是:%d\n",peach_num);
  return 0;
}