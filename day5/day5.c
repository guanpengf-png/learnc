#include <stdio.h>

int len = 5;
int arr[5] = {10, 20, 30, 40, 50};

void fn(int num)
{
    int a = 250;
    printf("%d \n", num + a);
}

int main(void)
{
    int i = 7;
    do {
        printf("%d", i);
        i++;
    } while (i < 10);

    /* 复用外面的 i，不要再写 for (int i = ...)，否则会盖住上面的 i */
    for (i = 0; i < 16; i++) {
        printf("%d", i);
    }

    char buff[100];
    int a = 10;
    int b = 8;
    sprintf(buff, "%d", a + b);
    printf("%s", buff);

    char *str = "Hello, World!";
    char *ptr = str;
    while (*ptr != '\0') {
        printf("%c", *ptr);
        ptr++;
    }
    printf("\n");
    printf("%s", str);
    printf("\n");

    int sum = 0;
    for (i = 0; i < 100; i++) {
        sum += 1;
        printf("%d", sum);
    }

    /* calculate the sum of the odd numbers from 1 to 100 */
    int sum1 = 0;
    for (i = 0; i < 100; i++) {
        if (i % 2 != 0) {
            sum1 += i;
        }
    }
    printf("%d", sum1);
    printf("\n");
    // calculate the sum of the even numbers from 1 to 100
    int sum2=0;
    for (int i=0;i<100;i++){
        if(i%2==0){
            sum2 +=i;
            
        }

    }
    printf("%d",sum2);
    return 0;
}
