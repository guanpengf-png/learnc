#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int *get_ran_array(int len);

int *get_ran_array(int len)
{
    static int num[10];
    srand((unsigned int)time(NULL));
    for (int i = 0; i < len; i++)
    {
        *(num+i)=rand()%50;
        }
        return &num[0];
    }
int main(){
   int *p=get_ran_array(10);
   printf("%p\n",p);
   printf("%d\n",*p);
   for (int i = 0; i < 10; i++)
   {
        printf("%d\n",*(p++));
   }
   
    return 0;

}