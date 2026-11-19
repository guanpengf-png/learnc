#include <stdio.h>
enum season
{
  spring,
  summer,
  autumn,
  winter
};
int main()
{
  // 需求:提示用户输入数字,显示喜欢的季节
  enum season s; // 枚举类型变量
  printf("请您输入季节:0---spring,1---summer,2---autumn,3---winter\n");
  scanf("%d",&s);
  switch(s){
    case spring: printf("您喜欢的是春天\n");break;
    case summer: printf("您喜欢的是夏天\n");break;
    case autumn: printf("您喜欢的是秋天\n");break;
    case winter: printf("您喜欢的是冬天\n");break;
  }


  return 0;
}