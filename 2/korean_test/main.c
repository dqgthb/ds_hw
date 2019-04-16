#include<stdio.h>

int main(void){
    unsigned char str[1024];
    scanf("%s", str);
    printf("%s\n", str);

    for(int i = 0; i < 20; ++i){
        printf("%x\t%u\n", str[i], str[i]);
    }

    return 0;
}
