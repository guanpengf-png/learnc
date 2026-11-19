#include <stdio.h>
int main()
{
  // 字符类型 char类型,由一个字母/一个数字/一个符号,使用单引号包裹起来,就是字符类型的数据值
  // 字符类型的数据值使用%c格式占位符进行输出
  // 字符值的本质是整型,字符类型的数据值也可以进行运算,但是不能过分
  // 字符值一共是128个,从0到127
  char ch1 = 'A'; // 声明一个char类型的变量,起名为ch1,赋值为'A'
  char ch2 = '9';
  printf("ch1=%c\n", ch1);
  printf("ch2=%c\n", ch2);
  char ch3 = '\t'; // 转义字符----\t----制表符
  printf("====%c=====\n", ch3);
  // printf("abc\t\tdefr\t\tfc\n");
  // printf("abcd3\t\tdef\t\tfc\n");
  printf("今天学习了一个\\");
  printf("今天学习了一个\"");
  char ch4 = '\t';
  char ch5 = 'A';
  printf("ch4+ch5的结果是:%c=======%d\n", ch4 + ch5, ch4 + ch5);

  // int num = 97;
  // printf("num=%d====%c\n",num,num);
  // printf("结果是:%c=====%d\n",'a'+'A','a'+'A');
  // 162----整数值  -128到+127

  // signed char ch1 = 200;
  // unsigned char ch2 = 200;
  // printf("ch1=%d,ch2=%d\n",ch1,ch2);

  // signed int num1 = -10;
  // unsigned int num2 = 5;
  // printf("结果:%u\n",num1+num2);

  printf("%c%c%c%c%c\n", 'h', 'e', 'l', 'l', 'o');

  return 0;
}