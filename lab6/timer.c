#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include"timer.h"
int main(int argc, char* argv[]){
    int len,i;
    if(argc>1 || argc ==0){
        usage();
    }
    else if(argc==1){
        if(atoi(argv[1])==0){
          usage();
        }
    }
}
void usage(){
  perror(" ");
  exit(1);
}
