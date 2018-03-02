CC      = gcc
CFLAGS  = -pedantic -Wall -g
LD      = gcc
LDFLAGS = 

mytar: mytar.o safe.o
	$(LD) $(LDFLAGS) mytar.o safe.o -o mytar

mytar.o: mytar.c mytar.h apue.h
	$(CC) $(CFLAGS) -c  mytar.c

safe.o: safe.c safe.h
	$(CC) $(CFLAGS) -c  safe.c

clean: 
	rm *.o mytar
