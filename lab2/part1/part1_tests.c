#include"checkit.h"
#include<stdio.h>
#include"part1.h"
int main(){
    int a=5,b=10;
    printf("a: %d, and b: %d",a,b);
    checkit_int(a,5);
    checkit_int(b,10);
    swap(&a,&b);
    printf("a: %d, and b: %d",a,b);
    checkit_int(a,10);
    checkit_int(b,5);
    return 0;
}