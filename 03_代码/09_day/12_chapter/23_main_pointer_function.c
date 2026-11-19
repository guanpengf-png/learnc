#include <stdio.h>
#include <string.h>
// 指针函数:
char * get_max_length_string(char *str1,char *str2){
  return strlen(str1) > strlen(str2) ? str1 : str2;
}
int main()
{
  // 指针函数:就是函数,返回值是指针,这样的函数叫指针函数
  // 指针函数:当一个函数的返回值是指针的时候,这个函数就是指针函数
  // 需求:通过计算两个字符串的长度比较,返回长度大的字符串
  char *str1 = "hello";
  char *str2 = "what are you no sha lei";
  char *max_str = get_max_length_string(str1,str2);
  printf("长度大的字符串是:%s\n",max_str);

  return 0;
}