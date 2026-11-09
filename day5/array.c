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

    return 0;
}