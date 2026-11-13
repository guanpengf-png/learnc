#include <stdio.h>
int main(){
    // int num=10;
    // int *p=&num;
    // printf("%d\n",num);
    // printf("%p\n",p);
    // printf("%p\n",&num);
    // printf("%p\n",&p);
    // printf("%d",*p);
    int numss[]={1,2,3,4,5};
    int *p1=&numss;
    printf("%p\n%zu",p1,sizeof(p1));
    int *p2=&numss[0];
    printf("%p\n%zu",p2,sizeof(p2));
    printf("%p\n%d\n%d",p1+1,*p1+1,(*p1)+1);
    printf("%d\n",numss[0]);
    (*p1)+1;
    printf("%d\n",numss[0]);


    

    return 0;
}