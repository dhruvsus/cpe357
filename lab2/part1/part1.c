#include<stdio.h>
#include"part1.h"
void swap(int* a, int* b){
    int temp=0;
    temp=*a;
    *a=*b;
    *b=temp;
}