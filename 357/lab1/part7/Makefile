CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = part7
OBJS = part7.o
all : $(MAIN)

$(MAIN) : $(OBJS) part7.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)
part7.o : part7.c part7.h
	$(CC) $(CFLAGS) -c part7.c

clean :
	rm *.o $(MAIN) core
