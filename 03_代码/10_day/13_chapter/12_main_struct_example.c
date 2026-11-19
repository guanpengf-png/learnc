#include <stdio.h>
#include <string.h>
// 定义游客的结构体类型
struct Visitor
{
  char name[30]; // 姓名
  int age;       // 年龄
  int price;     // 票价
};
// 计算票价
void computed_price(struct Visitor *ptr){
  ptr->price= ptr->age > 18 ? 20 : 0;
}
int main()
{
  // （1）一个景区根据游人的年龄收取不同价格的门票。
  // （2）请编写游人结构体（Visitor），根据年龄段决定能够购买的门票价格并输出。
  // （3）规则：年龄>18，门票为20元，其它情况免费。
  // （4）可以循环从控制台输入名字和年龄，打印门票收费情况，如果名字输入n，则退出程序。

  // 声明结构体类型的变量
  struct Visitor v;
  while (1)
  {
    printf("请您输入姓名:\n");
    scanf("%s", v.name);
    if (!strcmp(v.name,"n"))
      break;
    printf("请您输入年龄:\n");
    scanf("%d", &v.age);
    computed_price(&v); // 计算票价
    printf("%s今年%d岁,需要支付的票价是%d元\n",v.name,v.age,v.price);
  }
  printf("程序结束\n");

  return 0;
}