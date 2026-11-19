#include <stdio.h>
int main()
{
  // 多向分支语句:if-else if相关语句和switch-case语句的对比

  // 输入成绩范围,显示对应的级别:
  // 成绩在90(含)到100(含)之间,则显示A
  // 成绩在80(含)到90之间,则显示B
  // 成绩在60(含)到80之间,则显示C
  // 成绩在0(含)到60之间,则显示D
  // 使用if-else if和switch-case分别实现
  // int score = 88;
  // if(score>=90&&score<=100){
  //   printf("A\n");
  // }else if(score>=80){
  //   printf("B\n");
  // }else if(score>=60){
  //   printf("C\n");
  // }else {
  //   printf("D\n");
  // }

  // int score = 98;
  // // double score = 98.8;
  // switch(score/10){
  //   case 10:
  //   case 9: printf("A\n");break;
  //   case 8: printf("B\n");break;
  //   case 7: 
  //   case 6: printf("C\n");break;
  //   default : printf("D\n");break;
  // }

  // 输入级别,显示对应的分数范围
  // 输入A,则显示90(含)到100(含)之间
  // 输入B,则显示80(含)到90之间
  // 输入C,则显示60(含)到80之间
  // 输入D,则显示0(含)到60之间
  // char ch = 'A';
  // if(ch=='A'){
  //   printf("90到100之间\n");
  // }else if(ch=='B'){
  //   printf("80到90之间\n");
  // }else if(ch=='C'){
  //   printf("60到80之间\n");
  // }else{
  //   printf("0到60之间");
  // }
  // char ch = 'A';
  // switch(ch){
  //   case 'A':printf("90到100之间\n");break;
  //   case 'B':printf("80到90之间\n");break;
  //   case 'C':printf("60到80之间\n");break;
  //   case 'D':printf("0到60之间\n");break;
  // }

  // if-else if相关的语句和switch-case语句都可以实现范围和具体数据值的判断,都属于多向分支
  // if-else if相关语句更适合做范围的判断
  // switch-case相关语句更适合做具体数据值的判断
  return 0;
}