// Local library
#include "mips32.h"
#include "elf.h"

// std. library
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Error definetions
#define ERROR_INVALID_ARGS 1
#define ERROR_IO_ERROR 2
#define ERROR_READ_CONFIG_STREAM 3
#define ERROR_ELF_DUMP 4
#define ERROR_interp 5
#define ERROR_UNKNOW_OPCODE 6
#define ERROR_UNKNOW_FUNCT 7

#define syscall 8
#define interp_not_done 9


// MIPS32 Registers
#define ZR regs[0]
#define AT regs[1]
#define V0 regs[2]
#define V1 regs[3]
#define A0 regs[4]
#define A1 regs[5]
#define A2 regs[6]
#define A3 regs[7]
#define T0 regs[8]
#define T1 regs[9]
#define T2 regs[10]
#define T3 regs[11]
#define T4 regs[12]
#define T5 regs[13]
#define T6 regs[14]
#define T7 regs[15]
#define S0 regs[16]
#define S1 regs[17]
#define S2 regs[18]
#define S3 regs[19]
#define S4 regs[20]
#define S5 regs[21]
#define S6 regs[22]
#define S7 regs[23]
#define K0 regs[26]
#define K1 regs[27]
#define GP regs[28]
#define SP regs[29]
#define FP regs[30]
#define RA regs[31]

#define MEMSZ 640*1024

