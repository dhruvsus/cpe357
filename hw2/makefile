cc=gcc
cflags=-Wall -Werror -ansi -pedantic -g
objects=main.o fw_functions.o
all: main
main: main.o fw_functions.o
	$(cc) $(cflags) -o main $(objects)
main.o: fw_functions.h
fw_functions.o: fw_functions.h
clean: 
	rm main $(objects)
