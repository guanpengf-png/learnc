#include <stdio.h>
int main()
{
  // void * ptr; ptr的类型是void *类型
  // void*类型的指针可以指向任何类型的指针变量
  // 任何类型的指针都可以自动转换为void*类型
  // void*类型的指针也可以自动转换成任意的其他类型指针
  // void*类型的指针如果转其他类型的指针,建议使用强转

  int num = 10;
  // int *类型可以自动转成void*
  void *ptr = &num; // int *ptr = &num;
  printf("num的值是:%d,内存地址:%p\n",num,&num);
  printf("ptr指针变量指向的内存地址:%p\n",ptr);
  // printf("ptr指针变量指向的内存地址对应的内存空间中的数据:%d\n",*ptr); // 不能解引用
  // void*类型的指针可以转其他类型(自动)
  // int *ptr2 = ptr;
  // int *ptr2 = (int *)ptr; // 建议使用强制
  // void*类型的指针是不能解引用的
  // printf("ptr2指针变量指向的内存地址对应的内存空间中的数据:%d\n",*ptr2);

  double d_num = 12.34;
  void *ptr3 = &d_num;
  printf("d_num=%.2lf,对应的内存地址:%p\n",d_num,&d_num);
  printf("ptr====%p\n",ptr3);
  double *ptr4 = (double *)ptr3;
  printf("数据值是:%.2lf\n",*ptr4);
  // printf("*ptr3=%.2lf\n",*ptr3); // void *指针不能解引用

  return 0;
}