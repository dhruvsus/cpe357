#ifndef TIMER_H
#define TIMER_H
typedef void sigfunc(int);
void usage();
long int stringToNumber(const char* toNumber);
void startTimer(long int time);
#endif
