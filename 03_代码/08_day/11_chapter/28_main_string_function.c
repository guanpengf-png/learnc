#include <stdio.h>
#include <string.h>
int main()
{
  char chs1[] = "hello";
  printf("总字节数:%zu\n", sizeof(chs1)); // 6个
  printf("总长度:%zu\n", strlen(chs1));   // 5
  // strlen(字符数组/字符串); // 计算的是字符数组/字符串的长度,不包括\0
  printf("\n================\n");
  char chs3[50]; // 字符数组
  char chs4[] = "what are you no sha lei";
  char *str = strcpy(chs3, chs4);
  printf("chs3=%s,chs4=%s\n", chs3, chs4);
  // strcpy(参数1,参数2); 把参数2中的数据拷贝到参数1中
  printf("str===%s,str===%p\n", str, str);

  printf("\n================\n");

  char chs5[50] = "hello";
  char chs6[] = "every";
  char *str2 = strcat(chs5,chs6);
  printf("chs5=%s,chs6=%s\n",chs5,chs6);
  // strcat(参数1,参数2); 把参数2追加到参数1字符数组的后面

  return 0;
}