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
        printf("%d\t",i);

    }
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    printf("array[0]\t%d\n", array[0]);
    printf("array[length-1]\t%d\n", array[sizeof(array) / sizeof(array[0]) - 1]);

    int array1[3];
    array1[0] = 1;
    array1[1] = 2;
    array1[2] = 3;

    int array3[3] = {1, 2, 3};
    printf("array3[0]\t%d\n", array3[0]);
    printf("array3[1]\t%d\n", array3[1]);
    printf("array3[2]\t%d\n", array3[2]);

    int array4[3] = {1, 2, 3};
    printf("array4[0]\t%d\n", array4[0]);
    printf("array4[1]\t%d\n", array4[1]);
    printf("array4[2]\t%d\n", array4[2]);

    int array5[3] = {1, 2, 3};
    printf("array5[0]\t%d\n", array5[0]);
    printf("array5[1]\t%d\n", array5[1]);
    printf("array5[2]\t%d\n", array5[2]);

    return 0;
}
