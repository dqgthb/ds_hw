#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(void){
    char *str = "hello world!";
    char *str2 = strdup(str);
    if(!str2) exit(1);

    printf("%p %p\n", str, str2);
    printf("%s\n", str2);
    free(str2);

    return 0;
}
