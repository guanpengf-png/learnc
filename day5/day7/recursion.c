#include <stdio.h>
int get_fib(int a){
    if(a==1||a==2){
        return 1;
    }
    return get_fib(a-1)+get_fib(a-2);
    

}
int main(void)

{   
    int num=get_fib(12);
    
    
    return 0;
}
