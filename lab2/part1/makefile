CC=gcc
CFLAGS=-Wall -ansi -pedantic
all:part1_tests
part1_tests: part1_tests.o part1.o
	$(CC) -o part1_tests part1_tests.o part1.o
part1_tests.o: checkit.h part1.h
part1.o: part1.h
clean:
	rm *.o