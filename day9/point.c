// #include <stdio.h>
// int main(){
//     // int num=10;
//     // int *p=&num;
//     // printf("%d\n",num);
//     // printf("%p\n",p);
//     // printf("%p\n",&num);
//     // printf("%p\n",&p);
//     // printf("%d",*p);
//     int numss[]={1,2,3,4,5};
//     int *p1=&numss;
//     printf("%p\n%zu",p1,sizeof(p1));
//     int *p2=&numss[0];
//     printf("%p\n%zu",p2,sizeof(p2));
//     printf("%p\n%d\n%d",p1+1,*p1+1,(*p1)+1);
//     printf("%d\n",numss[0]);
//     (*p1)+1;
//     printf("%d\n",numss[0]);
//     int a =10,b=20,c=30;
//     int *prt[3];
//     prt[0]=&a;
//     prt[1]=&b;
//     prt[2]=&c;

//     int *prt2={&a,&b,&c};
//     int *a1=&a;
//     int *a2=&b;
//     int *a3=&c;
//     int *p3[]={a1,a2,a3};
//     int len=sizeof(p3)/sizeof(int *);
//     for (int i = 0; i <len; i++)
//     {
//         printf("index%d,%p,%d",i,p3[i],*p3[i],&p3[i]);
//     }
    

    



    

//     return 0;
// }
#include <stdio.h>

int main(void) {
    int numss[] = {1, 2, 3, 4, 5};
    int *p1 = numss;                    // 等价于 &numss[0]
    printf("%p\n%zu\n", (void *)p1, sizeof(p1));

    int *p2 = &numss[0];
    printf("%p\n%zu\n", (void *)p2, sizeof(p2));

    printf("%p\n%d\n%d\n", (void *)(p1 + 1), *p1 + 1, (*p1) + 1);
    printf("%d\n", numss[0]);
    (*p1) += 1;                         // 真正修改
    printf("%d\n", numss[0]);

    int a = 10, b = 20, c = 30;

    int *prt[3];                        // 先声明后赋值
    prt[0] = &a;
    prt[1] = &b;
    prt[2] = &c;

    int *prt2[] = {&a, &b, &c};         // 加 []，自动定长

    int *a1 = &a, *a2 = &b, *a3 = &c;
    int *p3[] = {a1, a2, a3};           // 花括号，不是圆括号

    int len = sizeof(p3) / sizeof(p3[0]);
    for (int i = 0; i < len; i++) {
        printf("index %d, value = %d, ptr = %p, ptr_addr = %p\n",
               i, *p3[i], (void *)p3[i], (void *)&p3[i]);
    }

    (void)prt; (void)prt2;    
    int n={1,2,3,4,5};
    int (*ns)[5]=&n;
    printf("%p for nums",&n);
    printf("%p for nums",ns);
    printf("%p\n,%d",ns+1,sizeof(ns)/sizeof(ns[0]));
    printf("&nums+1%p",&ns+1);

    
    // 消除未使用变量警告
    return 0;
}
