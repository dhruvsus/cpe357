cc=gcc
cflags=-Wall -Werror -ansi -pedantic -g
objects=fw.o fw_functions.o
all: fw
fw: fw.o fw_functions.o
	$(cc) $(cflags) -o fw $(objects)
fw.o: fw_functions.h
fw_functions.o: fw_functions.h
clean: 
	rm fw $(objects)
