#include <stdio.h>
#include <limits.h>
int get_nums_max(int*p,int len,int *max);
double get_averge(int*p,int len);
double get_averge(int*p,int len)
{
    int sum=1;
    double av=0.0;
    for (int i = 0; i < len; i++)
    {
        sum+=p[i];
    }
    av=sum/len;
    printf("%d",sum);
    return av;

    
}
int get_nums_max(int*p,int len,int *max){
    

    for (int i = 0; i < len; i++)
    {
        if (*max<*(p+i))
        {
            *max=*(p+i);
        }
       
        
    }
    return *max;
    




}
int main(){
    int num[]={1,2,3,4};
    int len =sizeof(num)/sizeof(int);
    int max=num[0];
    int max_num =get_nums_max(num,len,&max);
    double get_av=get_averge(num,len);

    printf("%d\n",max_num);
    printf("%lf\n",get_av);
    return 0;

}