cc= gcc
cflags= -Wall -pedantic -ansi -g
all: example
example: example.c example.o makefile
	$(cc) $(cflags) -o example example.o
example.o:
clean:
	rm *.o example
