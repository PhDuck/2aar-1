.PHONY: all clean

CC=gcc
CFLAGS=-Werror -Wall -Wextra -pedantic -std=c11

all: g1

g1: mips32.h g1.c
	$(CC) $(CFLAGS) -o g1 g1.c

clean:
	rm -f g1
