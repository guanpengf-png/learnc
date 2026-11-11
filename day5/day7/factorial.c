#include <stdio.h>
int ffactorial(int a){
    if (a==1)
    {
        return 1;
    }
    return a*ffactorial(a-1);
    
    
    
    

}
int main(void)

{   
    int result=ffactorial(5);
    printf("%d",result);
    
    return 0;
}
