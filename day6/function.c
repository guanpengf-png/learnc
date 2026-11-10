#include <stdio.h>
void say_hello(){
    printf("hello\n");
}
int get_sum(int a,int b){
    return a+b;
}
int get_maxnumber(int a,int b){
    return a>b?a:b;
}
int main(void)
{
    say_hello();
    int sum=get_sum(100,200);
    printf("sum: %d\n",sum);
    int a=10,b=20;
    int max_num=get_maxnumber(a,b);
    printf("max_getnumber: %d\n",max_num);
    printf("%s",__func__);
    return 0;
}