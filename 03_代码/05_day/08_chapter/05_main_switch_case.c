#include <stdio.h>
int main()
{
  // 多向分支语句之switch-case语句

  /*

    switch-case语句语法:

      switch(表达式){
        case 值1:语句1;break;
        case 值2:语句2;break;
        case 值3:语句3;break;
        ...
        default:语句4;break;
      }

      表达式:就是一个表达式,表达式的数据值的类型只能是"整型",不能是浮点数，可以是整型,也可以字符型
      语句1/语句2/语句3...:可以是一行代码也可以是多行代码
      default及后面的语句可以省略
      case后面的数据值必须是常量值

    switch-case语句执行过程:
      代码执行到switch(表达式)这一行的时候,计算出表达式的值,然后和case 后面的值1进行对比
      如果相同(类型和数据值),则执行语句1,遇到break则结束
      如果不相同,则和case后面的值2进行对比
      如果相同,则执行语句2,遇到break则结束
      如果不相同则和case后面的值3进行对比
      如果相同,则执行语句3,遇到break则结束
      直到和每个case后面的值进行对比都不相同,则执行default后面的语句4,遇到break则结束




    switch-case语句穿透现象:switch-case语句中的break关键字可以省略
    如果省略了break关键字,就会出现穿透现象
    穿透现象:代码执行到switch(表达式)这一行的时候,取出表达式的值和case后面的值进行对比,如果相同
    如果相同则执行语句,没有遇到break,则继续执行其他case后面的语句,直到遇到break才跳出switch-case语句
    如果不相同,则继续和其他case后面的值进行对比,如果都不相同,则执行default后面的语句,然后结束



  */
  // 例子:接收用户输入的字母,显示对应的星期,a--星期一
  char week='a';
  printf("请您输入字母,表示星期:\n");
  scanf("%c",&week);
  switch(week){
    case 'a': printf("星期一\n");break;
    case 'b': printf("星期二\n");break;
    case 'c': printf("星期三\n");break;
    case 'd': printf("星期四\n");break;
    case 'e': printf("星期五\n");break;
    case 'f': printf("星期六\n");break;
    case 'g': printf("星期日\n");break;
    default : printf("输入有误\n");break;
  }
  printf("程序结束\n");

  

  return 0;
}