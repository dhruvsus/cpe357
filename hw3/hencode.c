//
// Created by dhruv on 2/6/18.
//
#include<stdio.h>
#include "hencode.h"
#include<stdlib.h>
#include<errno.h>

int main(int argc, char* argv[]){
    if(argc<2){
        perror(usage());
        exit(EXIT_FAILURE);
    }
}
char* usage(){
    char* usage_error="usage: ./hencode infile [ outfile ]";
    return usage_error;
}