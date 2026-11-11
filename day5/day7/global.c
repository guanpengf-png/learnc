#include <stdio.h>
void f1(int a){
    if (a<3)
    {
        f1(a+1);
    }
    printf("->%d",a);
    

}
int main(void)

{   
    int num=1;
    f1(num);
    
    return 0;
}
