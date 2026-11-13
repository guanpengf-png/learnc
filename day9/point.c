#include <stdio.h>
int main(){
    int num=10;
    int *p=&num;
    printf("%d\n",num);
    printf("%p\n",p);
    printf("%p\n",&num);
    printf("%p\n",&p);
    printf("%d",*p);
    

    return 0;
}