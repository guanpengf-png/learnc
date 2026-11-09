#include <stdio.h>
#include <stddef.h>


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

    int row=3;
    int col=4;
    printf("{");
    for(int ii=0;ii<row;ii++){
        for(int jj=0;jj<col;jj++){
            //printf("[%d][%d]",ii,jj);
            if(jj<col-1){
                printf("[%d][%d]",ii,jj);
                printf(",");
            }
        }
        printf(",\n");

    }
    printf("}");

    }
    printf("%d",sum2);
   
    



    int nums[] = {10, 20, 30, 40, 50};
    int *ptr1 = &nums[0];
    int *ptr2 = &nums[3];

    // 指针差值：元素个数
    ptrdiff_t diff_ptr = ptr2 - ptr1;
    // 强转后，直接算地址字节差
    long long diff_byte = (long long)ptr2 - (long long)ptr1;

    printf("===== int 数组（合法：同一数组内）=====\n");
    printf("ptr2 - ptr1（元素个数）: %td\n", diff_ptr);
    printf("地址字节差: %lld\n", diff_byte);
    printf("验证：%lld / sizeof(int) = %lld\n\n", diff_byte, diff_byte / sizeof(int));


    double d1 = 1.0, d2 = 2.0;
    double *p1 = &d1, *p2 = &d2;

    ptrdiff_t diff_p = p2 - p1;
    long long diff_p_byte = (long long)p2 - (long long)p1;

    printf("===== double 独立变量（⚠️未定义行为！仅观察）=====\n");
    printf("p2 - p1（元素个数）: %td\n", diff_p);
    printf("地址字节差: %lld\n", diff_p_byte);
    printf("验证：%lld / sizeof(double) = %lld\n", diff_p_byte, diff_p_byte / sizeof(double));

    return 0;
}



