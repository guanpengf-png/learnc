#include <stdio.h>
enum season{
    a,
    b,
    c
};
int main(int argc, char const *argv[])
{
    enum season s;
    printf("choice season\n");
    scanf("%d",&s);
    switch (s)
    {
    case a:
        printf("a");
        break;
    case b:
        printf("b");
        break;
    case c:
        printf("c");
        break;
    
    }
    return 0;
}
