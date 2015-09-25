.PHONY: all clean

CC=gcc
CFLAGS=-Werror -Wall -Wextra -pedantic -std=c11

all: sim elf

elf: elf.c elf.h
	$(CC) $(CFLAGS) -c elf.c

sim: mips32.h elf.o sim.c
	$(CC) $(CFLAGS) -g -o sim elf.o sim.c

clean:
	rm -f sim
	rm -f *.o
