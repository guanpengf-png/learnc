#include <stdio.h>
#if _WIN32
    #include <windows.h>
    #define SLEEP(s) Sleep(s*1000)
#elif __linux__
    #include <unistd.h>
    #define SLEEP(s) sleep(s)
#endif
int main()
{
  // printf("%d\n",_WIN32);
  // printf("%d\n",__linux__);
  int num = 10;
  printf("程序运行了....\n");
  printf("数字结果是:%d\n",num);
  //暂停5秒
  SLEEP(5);
  printf("程序结束\n");

  return 0;
}