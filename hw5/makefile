cc=gcc
cflags=-Wall -Werror -ansi -pedantic -g
objects=parseline.h
all:parseline
parseline: parseline.o
	$(cc) $(cflags) parseline.o -o parseline
parseline.o: parseline.c parseline.h
