/*
 * 练习：水仙花数
 * 知识点：
 *   三位数的百位、十位、个位的立方和等于它自己。153 = 1^3 + 5^3 + 3^3。
 *   拆位：
 *     百位 = n / 100
 *     十位 = n / 10 % 10
 *     个位 = n % 10
 *   范围是 100 到 999，用循环逐个检查。
 */
#include <stdio.h>

int main(void)
{
    for (int number = 100; number <= 999; number++) {
        int hundreds = number / 100;
        int tens = number / 10 % 10;
        int ones = number % 10;
        int cube_sum = hundreds * hundreds * hundreds
            + tens * tens * tens
            + ones * ones * ones;
        if (cube_sum == number) {
            printf("%d\n", number);
        }
    }
    return 0;
}
