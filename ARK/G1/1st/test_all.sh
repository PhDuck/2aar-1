#!/bin/sh

set -e

for f in asm/*.elf
do 
  ./sim test.cfg $f
done
