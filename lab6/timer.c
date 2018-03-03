#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include"timer.h"
#include<limits.h>
#include<signal.h>
#include<errno.h>
int main(int argc, char* argv[]){
    long int time;
    if(argc!=2){
        usage();
        return 1;
    }
    time=stringToNumber(argv[1]);
    if(time<0){
        return 1;
    }
    else{
        startTimer(time);
        printf("Time’s up!\n");
    }
}
void usage(){
    fprintf(stderr,"usage: timeit <seconds>\n");
}
long int stringToNumber(const char* toNumber){
    long int time;
    char* endptr;
    errno=0;
    time=strtol(toNumber,&endptr,0);
    if(errno==ERANGE || *endptr!='\0' || endptr==toNumber){
        fprintf(stderr,"%s: malformed time.\n",toNumber);
        usage();
        return LONG_MIN;
    }
    return time;
}
void startTimer(long int time){
    errno=0;
    struct itimerval 
}
