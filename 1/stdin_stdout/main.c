#include<stdio.h>
#include<stdlib.h>

int main(void){
    printf("%p %p\n", &stdin, stdin);
    printf("%p %p\n", &stdout, stdout);
    printf("%lu\n", sizeof stdin);
    printf("%lu\n", sizeof *stdin);


    return 0;
}
