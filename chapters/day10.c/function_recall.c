#include <stdio.h>
#include<stdlib.h>
#include<time.h>
int get_rand(int x){
    srand((unsigned int)time(NULL));
    return rand()%x;

}
void printf_array(int *nums,int len){
    for (int i = 0; i < len; i++)
    {
        printf("%d\n",*(nums+i));
    }
    
}
int get_sum(int *arr,int len){
    int sum=0;
    for (int i = 0; i < len; i++)
    {
        sum+= *(arr+i);
    }
    return sum;
    
     
}
void change_array(int *nums,int len,int (*fun)(int)){
    for (int i = 0; i < len; i++)
    {
        *(nums+i)=fun(nums[i]);

    }
    
}
void change_array1(int *nums,int len,int (*fun)(int*,int)){
    for (int i = 0; i < len; i++)
    {
        *(nums+i)=fun(nums,len);

    }
    
}
int main(){
    int nums[]={1,2,3,4,5};
    int len=sizeof(nums)/sizeof(nums[0]);
    printf_array(nums,len);

    change_array(nums,len,get_rand);
    printf_array(nums,len);
    change_array1(nums,len,get_sum);
    printf_array(nums,len);

  
    return 0;
}
