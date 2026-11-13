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
    int a =10,b=20,c=30;
    int *prt[3];
    prt[0]=&a;
    prt[1]=&b;
    prt[2]=&c;

    int *prt2={&a,&b,&c};
    int *a1=&a;
    int *a2=&b;
    int *a3=&c;
    int *p3[]=(a1,a2,a3);

    



    

    return 0;
}