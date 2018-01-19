#include<stdio.h>
#include"part7.h"
int main(int argc, char* argv[]){
    int i=0;
    for(i=1;i<argc;i++){
        checkFirstChar(argv[i]);
    }
}
void checkFirstChar(char* cmdArgument){
    if(*cmdArgument=='-'){
        printf("%s\n",cmdArgument);
    }
}