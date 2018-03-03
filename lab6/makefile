cc=gcc
cflags=-Wall -ansi -g -pedantic
objects= timer.c
all:timer
timer: timer.o
	$(cc) $(cflags) -o timeit $(objects)
clean: 
	rm *.o timer
