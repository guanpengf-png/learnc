#include <stdio.h>
int main()
{
  // 字符指针:就是一个指针变量,指向的是字符/字符串/字符数组的内存地址
  // 字符指针的声明或定义方式:1.先声明后赋值,2.声明的同时直接赋值
  // 1. 先声明后赋值的写法:
  // char *chs1; // 声明了一个字符类型的指针,起名为chs1
  // chs1 = "hello";
  // printf("chs1指向的内存地址:%p=======chs1对应的内容是:%s\n",chs1,chs1);
  // // 2. 声明的同时直接赋值的写法:
  // char *chs2 = "every";
  // printf("chs2指向的内存地址:%p=======chs2对应的内容是:%s\n",chs2,chs2);
  // // 声明字符数组并赋值:
  // char chs3[] = "what are you no sha lei";
  // printf("chs3字符数组的内存地址:%p======chs3对应的内容是:%s\n",&chs3,chs3);

  char *chs1 = "hello";  // 字符指针
  char chs2[] = "world"; // 字符数组
  // 字符指针和字符数组都可以看成是在存储字符串数据,索引,都可以使用for循环遍历数据
  // 也都可以使用%s格式占位符进行输出
  // 字符指针可以重新赋值,字符数组不能重新赋值(不是指针,不能改变指向,也不能改变长度)
  printf("chs1=%s,chs2=%s\n", chs1, chs2);
  printf("\n=============\n");
  for (int i = 0; i < 5; i++)
    printf("%c\n", chs1[i]);
  printf("\n=============\n");
  for (int i = 0; i < 5; i++)
    printf("%c\n", chs2[i]);
  // 修改里面的数据:
  // printf("chs1====%p\n", chs1);
  // chs1 = "what are you no sha lei"; // 字符指针重新赋值的本质意义是修改了指针的指向
  // printf("chs1====%p\n", chs1);
  // chs2 = "o good"; // 字符数组不能重新赋值
  // chs1[0] = 'A'; // 字符指针是不能通过索引的方式修改某个位置的数据的(那个数据属于只读的,在常量区)
  // printf("chs1====%p\n", chs1);
  chs2[0] = 'A'; // 字符数组是可以通过索引修改某个位置的数据的
   printf("chs2====%s\n", chs2); 
  return 0;
}