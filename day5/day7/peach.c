#include <stdio.h>
int get_peach(int a){
    if(a==10){
        return 1;
    }
    return (get_peach(a+1)+1)*2;

  
    

}
int main(void)

{   
    int peach_num=get_peach(1);
    printf("%d",peach_num);
    
    return 0;
}
