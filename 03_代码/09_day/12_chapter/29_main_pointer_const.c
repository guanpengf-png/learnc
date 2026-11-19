#include <stdio.h>
int main()
{
  // 指针const:也是指针,也是使用const关键字进行修饰,const在*的右侧
  // 数据类型名 *const 指针名= &变量;
  // 指针const是不能修改指向的,可以解引用修改变量的值

  int num = 10;
  int *const ptr = &num;
  int num2 = 20;
  printf("num的值是:%d,对应的内存地址是:%p\n",num,&num);
  printf("ptr指针指向的内存地址对应的空间中的数据是:%d\n",*ptr);
  printf("ptr指针指向的内存地址:%p\n",ptr);
  *ptr += 100; // 指针const可以通过解引用修改变量的值
  // ptr = &num2; // 指针const不可以修改指向
  printf("num的值是:%d,对应的内存地址是:%p\n", num, &num);
  printf("ptr指针指向的内存地址对应的空间中的数据是:%d\n",*ptr);
  printf("ptr指针指向的内存地址:%p\n",ptr);


  /*
    const指针和指针const的区别:
    前者可以修改指向,后者不能修改指向
    前者不能解引用修改变量的值,后者可以解引用修改变量的值
    前者的const在*的左侧,后者的const在*的右侧
  
  
  */

  return 0;
}