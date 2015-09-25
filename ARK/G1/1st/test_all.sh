#!/bin/sh

set -e

make clean
make

cd asm/

make clean
make

cd ..

for f in asm/*.elf
do 
  ./sim test.cfg $f
done
