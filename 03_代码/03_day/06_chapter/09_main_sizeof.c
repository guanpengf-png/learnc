#include <stdio.h>
int main()
{

  size_t
  // sizeof是一个关键字,用来计算数据类型/变量/常量/数据值所占的字节数
  // sizeof关键字的使用书写方式两种:
  // 方式1:  sizeof(数据类型名/变量名/数据值)
  // 方式2:  sizeof 变量名/数据值
  // sizeof计算的结果使用%zu格式占位符进行输出即可

  // 1. 使用sizeof计算基本数据类型所占的字节数
  printf("字符型 char 所占的字节数:%zu\n", sizeof(char));           // 1
  printf("型 short 所占的字节数:%zu\n", sizeof(short));             // 2
  printf("型 int 所占的字节数:%zu\n", sizeof(int));                 // 4
  printf("型 long 所占的字节数:%zu\n", sizeof(long));               // 4
  printf("型 long long 所占的字节数:%zu\n", sizeof(long long));     // 8
  printf("型 float 所占的字节数:%zu\n", sizeof(float));             // 4
  printf("型 double 所占的字节数:%zu\n", sizeof(double));           // 8
  printf("型 long double 所占的字节数:%zu\n", sizeof(long double)); // 16

  printf("\n======================\n");
  // 2. 使用sizeof计算不同类型变量所占的字节数
  char ch = 'A';
  short s = 23;
  int num = 10;
  long l = 20L;
  long long ll = 30LL;
  float f = 23.45f;
  double d = 12.34;
  long double ld = 123.456L;
  printf("ch 变量 所占的字节数是:%zu\n", sizeof ch);    // 1
  printf("s 变量 所占的字节数是:%zu\n", sizeof(s));     // 2
  printf("num 变量 所占的字节数是:%zu\n", sizeof(num)); // 4
  printf("l 变量 所占的字节数是:%zu\n", sizeof(l));     // 4
  printf("ll 变量 所占的字节数是:%zu\n", sizeof ll);    // 8
  printf("f 变量 所占的字节数是:%zu\n", sizeof(f));     // 4
  printf("d 变量 所占的字节数是:%zu\n", sizeof(d));     // 8
  printf("ld 变量 所占的字节数是:%zu\n", sizeof(ld));   // 16

  printf("\n======================\n");
  // 3. 使用sizeof计算字面量数据值所占的字节数
  // 字符字面量值本质是整型,所以占4个字节
  printf("字符字面量值 'A' 所占的字节数:%zu\n",sizeof('A')); // 4
  printf("整数字面量值 100 所占的字节数:%zu\n",sizeof(100) ); // 4
  printf("单精度浮点数字面量值 12.34f 所占的字节数:%zu\n",sizeof(12.34f) ); // 4
  printf("双精度浮点数字面量值 12.34 所占的字节数:%zu\n",sizeof(12.34) ); // 8
  printf("长双精度浮点数字面量值 12.34L  所占的字节数:%zu\n",sizeof(12.34L) ); // 16



  return 0;
}