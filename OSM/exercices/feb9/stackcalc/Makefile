.PHONY: clean

CC=gcc
CFLAGS=-Werror -Wextra -pedantic -std=c11

all: calc

calc: stack.o calc.c
	$(CC) $(CFLAGS) -o calc stack.o calc.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

clean:
	rm *.o
	rm calc
