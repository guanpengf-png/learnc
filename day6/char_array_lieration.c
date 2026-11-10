#include <stdio.h>

int main(void)
{
    int num[2][3];
        num[0][0]=1;
        num[0][1]=2;
        num[0][2]=3;
        num[1][0]=4;
        num[1][1]=5;
        num[1][2]=6;
    int num1[2][3]={
        {1,2,3},
        {4,5,6},
    };
    printf("num[0][0]: %d\n", num[0][0]);
    printf("num[0][1]: %d\n", num[0][1]);
    printf("num[0][2]: %d\n", num[0][2]);
    printf("num[1][0]: %d\n", num[1][0]);
    printf("num[1][1]: %d\n", num[1][1]);
    printf("num[1][2]: %d\n", num[1][2]);
    printf("num1[0][0]: %d\n", num1[0][0]);
    printf("num1[0][1]: %d\n", num1[0][1]);
    printf("num1[0][2]: %d\n", num1[0][2]);
    printf("num1[1][0]: %d\n", num1[1][0]);
    printf("num1[1][1]: %d\n", num1[1][1]);
    printf("num1[1][2]: %d\n", num1[1][2]);
    int num3[][3]={
        {1,2,3},
    };
    int num4[][4]={1,2,3,4};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            printf("num3[%d][%d]: %d\n", i, j, num3[i][j]);
        }
    }
    int row = sizeof(num4)/sizeof(num4[0]);
    int col=4;
    for (int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            printf("num4[%d][%d]: %d\n", i, j, num4[i][j]);

        }

    }
   


    return 0;
}


        
   

