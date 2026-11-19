#include <stdio.h>
#include <string.h>
int main()
{
  // 字符数组的访问有两种方式:
  char chs[] = "hello";
  // 方式1:使用%s格式占位符的方式进行获取及输出
  printf("%s\n", chs);
  printf("\n====================\n");
  // 方式2:使用%c格式占位符及遍历的形式进行获取及输出
  int len1 = sizeof(chs) / sizeof(chs[0]);
  // int len = sizeof(chs) / sizeof(char);
  // 因为字符所占的字节数是1个,所以,下面的写法也是可以的
  // int len = sizeof(chs);
  // int len = sizeof(chs) / sizeof(char) - 1; // -1的目的是去掉\0的遍历
  int len2 = strlen(chs);
  printf("len1=%d,len2=%d\n", len1, len2);
  for (int i = 0; i < len2; i++)
  {
    printf("索引:%d,元素值:%c\n", i, chs[i]);
  }

  return 0;
}