#include<stdio.h>
#include<stdlib.h>
#include"fw_functions.h"
#include<unistd.h>
#include<string.h>
int main(int argc, char* argv[]){
    int files=0,n=10;
    char *nonNum;
    if(argc==1){
        files=-1;
        printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
        return 1;
    }
    else if(strcmp(argv[1],"-n")==0){
        files=3;
        if(argc==2){
            printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
            return 1;
        }
        n=strtol(argv[2],&nonNum,0);
        if(isalphanum(*nonNum))
    }
}
