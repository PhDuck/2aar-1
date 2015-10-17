#! /bin/sh

set -e

make clean
make

cd asm

#make clean
#make

cd ..

./sim default.cfg asm/sw-lw.elf
./sim default.cfg asm/add.elf
./sim default.cfg asm/beq-true-nopsled.elf
./sim default.cfg asm/beq-false-nopsled.elf
./sim default.cfg asm/j-nopsled.elf
./sim default.cfg asm/jal-nopsled.elf
./sim default.cfg asm/jal-jr-j-nopsled.elf

