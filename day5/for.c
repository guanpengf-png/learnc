#include <stdio.h>
int main(void)
{
    for(int i=1;i<=9;i++){
        for(int j=1;j<=9-i;j++)
        {
            printf(" ");
        }
        for(int j=1;j<=i;j++){
            printf("%d*%d=%d",i,j,i*j);

        }
        printf("\n");

    }
    for (int i=9;i>=1;i--){
        for(int j=1;j<=9-i;j++){
            printf(" ");
        }
        for(int j=1;j<=i;j++){
            printf("%d*%d=%d\t",i,j,i*j);
        }
        printf("\n");
    }
    for(int i=0;i<=100;i++){
        if(i%7==0||i/10==7||i%10==7){
            continue;
        }
        printf("%d",i);

    }
    return 0;
}