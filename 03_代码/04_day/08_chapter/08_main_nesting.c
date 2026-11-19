#include <stdio.h>
int main()
{
  // 嵌套分支:分支语句中还可以有其他的分支语句
  /*
    if语句中可以有if语句/if-else语句/if-else if语句/switch-case语句/三元表达式
    if-else语句中可以有if语句/if-else语句/if-else if语句/switch-case语句/三元表达式
    ifelse if语句中可以有if语句/if-else语句/if-else if语句/switch-case语句/三元表达式
    switch-case语句中可以有if语句/if-else语句/if-else if语句/switch-case语句/三元表达式
    三元表达式中可以有三元表达式


  */

  // 例子:输入油号（92或95）和油量，计算需要支付的金额。92号汽油每升4元，如果油量大于等于40升，每升3.5元；95号汽油每升5元，如果油量大于等于40升，每升4.5元。

  int oilN = 0, oilL = 0; // 声明两个int类型的变量,起名为oilN和oilL表示油号和油量
  printf("请您输入油号和油量:\n");
  scanf("%d %d", &oilN, &oilL); // 接收数据并存储起来
  double price = 4.0;
  switch (oilN)
  {
  case 92:
    //  price = 4.0; // 价格
    if (oilL >= 40)
      price = 3.5;
    break;
    // printf("支付金额是:%.2lf\n", price * oilL);break;
  case 95:
    price = 5.0; // 价格
    if (oilL >= 40)
      price = 4.5;
    break;
    // printf("支付金额是:%.2lf\n", price * oilL);break;
  }
  printf("支付金额是:%.2lf\n", price * oilL);

  return 0;
}