#include <stdio.h>
#include<string.h>

char* swap_get_max(char* ch,char *ch1);
int strlenn(char* ch){
    int len=0;
    while (*ch != '\0')
    {
        len++;
        ch++;
    }
    return len;
}

char* swap_get_max(char* ch,char *ch1){
    return strlen(ch)>strlen(ch1)?ch:ch1;
}
int main(){
    char* ch="abc";
    char* ch1="abcde";
    char*max=swap_get_max(ch,ch1);
    int len=strlenn(ch);
    printf("%d",len);
    printf("%s",max);
    return 0;

}