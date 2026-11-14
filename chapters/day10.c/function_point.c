#include <stdio.h>
int get_sum(int x,int y){
    return x+y;
}
int get_sub(int x,int y){
    return x-y;
}


int main(){
    int sum1=get_sum(1,2);
    printf("%d\n",sum1);

    // 函数名本身就是函数地址，可以直接赋值
    int (*p)(int,int)=get_sum;      // 等价于 = &get_sum;
    int (*p1)(int,int)=&get_sum;    // 显式取地址也可以

    // 通过函数指针调用
    printf("%d\n",p(3,1));          // 4
    printf("%d\n",(*p1)(9,5));      // 14，两种调用写法等价

    // 重新指向另一个函数：赋的是函数地址，不是调用结果
    p=get_sub;
    printf("%d\n",p(3,1));          // 2

    return 0;
}
