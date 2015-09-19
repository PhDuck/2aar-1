#!/bin/sh

set -e

cd asm/

make clean
make

cd ..

for f in asm/*.elf
do 
  ./sim test.cfg $f
done
