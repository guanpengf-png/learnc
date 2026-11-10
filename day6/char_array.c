#include <stdio.h>

int main(void)
{
    char char1[5]={'h','e','l','l','\0'};

    char char2[4]={'a','b','c'};
    char char3[]={'a','c','\0'};
    char char4[]={'a','b','c'};
    char char5[]="abc";
    char char6[]={"abc"};
    printf("char1: %s\n", char1);
    printf("char2: %s\n", char2);
    printf("char3: %s\n", char3);
    printf("char4: %s\n", char4);
    printf("char5: %s\n", char5);
    printf("char6: %s\n", char6);
    return 0;
}
