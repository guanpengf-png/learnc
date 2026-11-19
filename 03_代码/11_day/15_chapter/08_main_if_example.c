#include <stdio.h>
#if __STDC_VERSION__ > 199901
#define FORMAT_INTEGERT long long
#define FORMAT_TYPE "%lld"
#else
#define FORMAT_INTEGERT long
#define FORMAT_TYPE "%ld"
#endif
int main()
{
  // 199901=======201710
  // 需求:如果当前c的版本在c99之后,声明变量的类型可以使用long long,否则使用的是long类型声明变量
  // printf("版本:%ld\n", __STDC_VERSION__);

  // 数据类型 变量 = 值;
  FORMAT_INTEGERT num = 100;
  printf("结果是:"FORMAT_TYPE"\n",num);
  printf("字节数:%zu\n",sizeof(num));

  return 0;
}