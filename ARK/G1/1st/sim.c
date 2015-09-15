#include <stdio.h>
#include "mips32.h"
#include <stdlib.h>
#include <stdint.h>
#include "elf.h"

#define ERROR_INVALID_ARGS 1
#define ERROR_IO_ERROR 2
#define ERROR_READ_CONFIG_STREAM 3

#define AT regs[0]
#define V0 regs[1]
#define V1 regs[2]
#define T0 regs[3]
#define T1 regs[4]
#define T2 regs[5]
#define T3 regs[6]
#define T4 regs[7]
#define T5 regs[8]
#define T6 regs[9]
#define T7 regs[10]
#define SP regs[11]
#define RA regs[12]

#define MEMSZ 640*1024

static uint32_t PC;
static size_t instr_cnt;
static uint32_t regs[32];
static unsigned char mem[MEMSZ];


int show_status() 
{
    printf("Executed %zu instrucitons(s).\n", instr_cnt);
    printf("pc = 0x%x\n", PC);
    printf("v0 = 0x%x\n", V0);
    printf("v1 = 0x%x\n", V1);
    printf("t0 = 0x%x\n", T0);
    printf("t1 = 0x%x\n", T1);
    printf("t2 = 0x%x\n", T2);
    printf("t3 = 0x%x\n", T3);
    printf("t4 = 0x%x\n", T4);
    printf("t5 = 0x%x\n", T5);
    printf("t6 = 0x%x\n", T6);
    printf("t7 = 0x%x\n", T7);
    printf("sp = 0x%x\n", SP);
    printf("ra = 0x%x\n", RA);

  return 0;
}

  
 int read_config_stream(FILE *stream)
{
 uint32_t v;

 for (int i=0; i <= 7; ++i)
   {
    if (fscanf(stream, "%u", &v) != 1)
      return ERROR_READ_CONFIG_STREAM;
    regs[i + 3] = v;  
   }
  
  return 0;  
}


int read_config (const char *path)
{
 FILE *stream = fopen(path, "r");
 
 if (stream == NULL)
   return ERROR_IO_ERROR;
 else if (read_config_stream(stream) != 0)
   return ERROR_READ_CONFIG_STREAM;
 if (fclose(stream) != 0)
   return ERROR_IO_ERROR;
   
 return 0;
}



int main(int argc, char const *argv[])
{

  if (argc == 3) {
    read_config(argv[1]);
  } else {
    printf("Moron, more or less arguments!\n");
    return ERROR_INVALID_ARGS;
  }

  

  elf_dump(argv[2], &PC, &mem[0], MEMSZ);
  show_status();
  
  return 0;
  }
