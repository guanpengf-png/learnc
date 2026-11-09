#include <stdio.h>

int main() {
    int num[]={1,2,3,4,5,6,7,8,9,10};
    printf("total of nums size:%d\n",sizeof(num));
    printf("total of length arrray element:%d\n",sizeof(num[0]));
    int len=sizeof(num)/sizeof(num[0]);
    printf("total of length array element:%d\n",len);
    for(int i=0;i<len;i++){
        printf("%d\t",num[i]);
    }
    int max_num=num[0];
    int min_num=num[0];
    int max_index=0;
    int min_index=0;
    int sum=0;
    for(int i=0;i<len;i++){
        sum+=num[i];
        if(max_num>num[i]){
            max_num=num[i];
            max_index=i;
        }
        if(min_num<num[i]){
            min_num=num[i];
            min_index=i;
        }
    }
    printf("max_num:%d\n",max_num);
    printf("max_index:%d\n",max_index);
    printf("min_num:%d\n",min_num);
    printf("min_index:%d\n",min_index);
    printf("sum:%d\n",sum);
    return 0;
}