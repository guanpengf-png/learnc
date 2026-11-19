#include <stdio.h>
// 主函数的理解:
/*

1.主函数是在程序运行的时候自动调用的
2.主函数有且只能有一个
3.主函数中可以调用其他函数
4.不要在其他函数中调用主函数,否则会形成死循环
5.主函数中也不要调用主函数字节,否则也会形成死循环
6.主函数是有返回值类型的,是int,也有参数

*/
// argc---argument count---参数的个数---表示数组中数据的个数
// argv---argument value---参数的数据值--数组中数据的元素值
// int main(int argc, char const *argv[])
// {
//   printf("argc value is :%d\n",argc);
//   for (int i = 0; i < argc;i++){
//     printf("argv[%d] value is:%s\n",i,argv[i]);
//   }
//     return 0;
// }
#include <stdio.h>
int main()
{
  
  
  return 0;
}

// int main()
// {
//   printf("main函数\n");
//   return 0;
// }

// void f1(){
//   printf("f1函数\n");
//   // main();
// }
// int main()
// {
//   printf("main主函数\n");
//   // main();
//   // f1();
//   return 0;
// }
