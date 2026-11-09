#include <stdio.h>
int main(void)
{
    for (int i=0;i<19;i++){
        for(int j=0;j<=i;j++){
            printf("%d*%d=%d",i,j,i*j);
            if(j<i){
                printf(",");
            }
        }
        printf("\n");
    }
    return 0;
}