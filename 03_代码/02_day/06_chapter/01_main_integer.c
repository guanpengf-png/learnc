#include <stdio.h>

int main()
{

  {
    // 短整型 short,分为有符号的short和无符号的short
    signed short s1 = -10;   // 声明一个有符号的short类型的变量,起名为s1,并赋值为-10
    unsigned short s2 = 20u; // 声明一个无符号的short类型的变量,起名为s2,并赋值为20
    // unsigned short s2 = 20; // 声明一个无符号的short类型的变量,起名为s2,并赋值为20
    short s3 = -20; // 声明一个有符号的short类型的变量,起名为s3,并赋值为-20
    // signed表示有符号的,声明变量的时候signed可以省略,默认就是有符号的
    // 无符号的数据值后面的后缀是u/U,也可以省略
    // 有符号的short类型的数据输出使用的格式占位符是%hd,无符号的short类型的数据输出使用的是%hu
    // 无论是有符号的还是无符号的short类型数据都可以使用%d格式占位符,但是建议使用各自
    printf("s1变量的值是:%hd\n", s1);
    printf("s2变量的值是:%hu\n", s2);
    printf("s3变量的值是:%hd\n", s3);
    printf("s1=%d,s2=%d,s3=%d\n", s1, s2, s3);
  }
  printf("\n================================\n");
  {
    // 整型int,分为:有符号的和无符号的int
    signed int num1 = -100;  // 声明一个有符号的int类型的变量,起名为num1,并赋值-100
    unsigned int num2 = 200; // 声明一个无符号的int类型的变量,起名为num2,并赋值200
    int num3 = -200;         // 声明一个有符号的int类型的变量,起名为num3,并赋值-200
    // 有符号的signed也是可以省略的
    // 有符号的int类型的数据输出使用的是%d格式占位符,无符号的int类型的数据输出使用的是%u格式占位符
    // 无论是有符号的int还是无符号的int类型的数据输出都可以使用%d,建议还是各自的格式占位符
    printf("num1=%d\n", num1);
    printf("num2=%u\n", num2);
    printf("num3=%d\n", num3);
    printf("num1=%d,num2=%d,num3=%d\n", num1, num2, num3);
  }
  printf("\n================================\n");
  {
    // 长整型 long,分为有符号的long和无符号的long
    signed long l1 = -1000L;   // 声明一个有符号的long类型的变量,起名为l1,并赋值为-1000
    unsigned long l2 = 2000LU; // 声明一个无符号的long类型的变量,起名为l2,并赋值2000
    long l3 = -2000L;          // 声明一个有符号的long类型的变量,起名为l3,并赋值为-2000
    // 有符号的long类型的数据值的后缀是l/L
    // 无符号的long类型的数据值的后缀是lu/LU/ul/UL
    // long类型的数据值的后缀无论是L还是LU都可以省略,不是很建议
    // signed关键字也可以省略
    // 赋值的时候的后缀在输出的时候后缀是没有的
    // 有符号的long类型的数据输出使用的格式占位符是%ld,无符号的格式占位符是%lu
    // long类型的数据输出的时候也可以使用%d格式占位符,建议使用各自的
    printf("l1的值是:%ld\n", l1);
    printf("l2的值是:%lu\n", l2);
    printf("l3的值是:%ld\n", l3);
    printf("l1=%d,l2=%d,l3=%d\n",l1,l2,l3);
  }
  printf("\n================================\n");
  {
    // 长长整型 long long,也分为有符号的和无符号的
    signed long long ll1 = -10LL; // 声明一个有符号的long long类型的变量,起名为ll1,赋值-10
    unsigned long long ll2 = 20LLU; // 声明一个无符号的long long类型的变量,起名为ll2,赋值20
    long long ll3 = -20LL; //  声明一个有符号的long long类型的变量,起名为ll3,赋值-20
    // 有符号的long long类型的数据值的后缀是ll/LL,建议使用大写
    // 无符号的long long类型的数据值的后缀是llu/ull/LLU/ULL
    //后缀也可以省略,不是很建议
    // signed也可以省略
    // 有符号的long long类型的数据输出使用的格式占位符是%lld
    // 无符号的long long类型的数据输出使用的格式占位符是%llu
    // 无论是有符号的long long还是无符号的long long类型的数据也可以使用%d格式占位符,但是建议使用各自的
    printf("ll1变量的值是:%lld\n",ll1);
    printf("ll2变量的值是:%llu\n",ll2);
    printf("ll3变量的值是:%lld\n",ll3);
    printf("ll1=%d,ll2=%d,ll3=%d\n",ll1,ll2,ll3);
  }

  return 0;
}