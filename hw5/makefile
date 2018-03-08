cc=gcc
cflags=-Wall -Werror -ansi -pedantic -g
objects=parseline.h
all:parseline
parseline: parseline.o safe.o
	$(cc) $(cflags) parseline.o safe.o -o parseline
parseline.o: parseline.c parseline.h
safe.o: safe.c safe.h
