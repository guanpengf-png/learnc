#include <stdio.h>
#include <time.h>
void f1(){
  int sum = 0;
  for (int i = 0; i < 100000;i++){
    for (int j = 0; j < 10000;j++){
      sum += i;
    }
  }
}
int main()
{
  // time_t current_time;
  // time(&current_time); // 计算的是秒
  // printf("%lld\n",current_time);

  // time_t current_time;
  // time(&current_time); // 秒
  // printf("%s\n",ctime(&current_time)); // 带有年月日星期和小时分钟秒的时间

  time_t start_time, end_time;
  time(&start_time); // 开始时间
  f1();
  time(&end_time); // 结束时间
  double result= difftime(end_time, start_time); // 计算时间差
  printf("%.2lf\n",result);

  return 0;
}