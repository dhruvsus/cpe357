//
// Created by dhruv on 2/6/18.
//
#include<stdio.h>
#include "hencode.h"
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#define ASCII 256
int main(int argc, char* argv[]){
    if(argc<2 || argc>3){
        usage();
        exit(EXIT_FAILURE);
    }
    else if(argc==2){
        printf("Output mode: stdout\n");
    }
    else if(argc==3){
        printf("Output mode: file: %s\n",argv[2]);
    }
    else{
        return 1;
    }
    build_array(argv[1]);
}
void usage(){
    printf("usage: ./hencode infile [ outfile ]\n");
}
void build_array(char* filename){
    int* chartable;
    int fd_read;
    chartable=(int*)calloc(ASCII,sizeof(int));
    fd_read=open(filename,O_RDONLY);
    if(fd_read==-1){
        printf("%s\n",strerror(errno));
    }
    else{

    }
}