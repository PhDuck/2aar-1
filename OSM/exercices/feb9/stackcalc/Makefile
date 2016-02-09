.PHONY: clean

CC=gcc
CFLAGS=-Werror -Wextra -pedantic -std=c11

all: calc

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

calc: stack.o calc.c
	$(CC) $(CFLAGS) -o calc stack.o calc.c

clean: stack.o calc
	rm *.o
	rm calc