#include "mips32.h"
#include "elf.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define ERROR_INVALID_ARGS 1
#define ERROR_IO_ERROR 2
#define ERROR_READ_CONFIG_STREAM 3
#define ERROR_ELF_DUMP 4
#define ERROR_interp 5
#define ERROR_UNKNOW_OPCODE 6
#define ERROR_UNKNOW_FUNCT 7

#define syscall 8
#define interp_not_done 9

#define AT regs[1]
#define V0 regs[2]
#define V1 regs[3]
#define T0 regs[4]
#define T1 regs[5]
#define T2 regs[6]
#define T3 regs[7]
#define T4 regs[8]
#define T5 regs[9]
#define T6 regs[10]
#define T7 regs[11]
#define SP regs[12]
#define RA regs[13]

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
    regs[i + 4] = v;  
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

int intep_r(uint32_t inst)
{
  uint32_t rs = GET_RS(inst);
  uint32_t rt = GET_RT(inst);
  uint32_t shamt = GET_SHAMT(inst);
  uint32_t funct = GET_FUNCT(inst);
  //uint32_t rd = GET_RD(inst);
  uint32_t *rdd = &regs[RA];

  printf("%x\n", GET_FUNCT(inst));

  switch (funct)
  {
  case FUNCT_JR:
    printf("JR\n");
    PC = rs;
    break;
  case FUNCT_ADDU:
    printf("ADD\n");
    *rdd = rs + rt;
    break;
  case FUNCT_SUBU:
    printf("SUBU\n");
    *rdd = rs - rt;
    break;
  case FUNCT_AND:
    printf("AND\n");
    *rdd = rs & rt;
    break;
  case FUNCT_OR:
    printf("OR\n");
    *rdd = rs | rt;
    break;
  case FUNCT_NOR:
    printf("NOR\n");
    *rdd = !(rs | rt);
    break;
  case FUNCT_SLT:
    printf("SLT\n");
    *rdd = (rs < rt) ? 1 : 0;
    break;
  case FUNCT_SLL:
    printf("SLL\n");
    *rdd = rt << shamt;
    break;
  case FUNCT_SRL:
    printf("SRL\n");
    *rdd = rt >> shamt;
    break;
  case FUNCT_SYSCALL:
    printf("SYSCALL FOUND!\n");
    return syscall;
  default:
    return ERROR_UNKNOW_FUNCT;
  }
  return 0;
}


int interp_inst(uint32_t inst) 
{
   uint32_t result;
   uint32_t test = GET_BIGWORD(mem, PC);

  printf("%x", GET_OPCODE(inst));
  switch (GET_OPCODE(inst))
  {
  case OPCODE_R:
    printf("OPCODE_R\n");
    result = intep_r(inst);
    printf("%x\n", result); 
    printf("%x\n", test);
    break;
  case OPCODE_J:
    printf("Nope");
    break;
  case OPCODE_JAL:
     printf("Nope");
   break;
  case OPCODE_BEQ:
      printf("Nope");
  break;
  case OPCODE_BNE:
       printf("Nope");
 break;
  case OPCODE_ADDIU:
     printf("Nope");
   break;
  case OPCODE_SLTI:
     printf("Nope");
   break;
  case OPCODE_ANDI:
     printf("Nope");
   break;
  case OPCODE_ORI:
     printf("Nope");
   break;
  case OPCODE_LUI:
     printf("Nope");
   break;
  case OPCODE_LW:
     printf("Nope");
   break;
  case OPCODE_SW:
     printf("Nope");
   break;
  default:
    return ERROR_UNKNOW_OPCODE;
  }
  
  return interp_not_done;
}

int interp()
{
  while(1)
  {
    instr_cnt++;
    uint32_t value;
    uint32_t inst = GET_BIGWORD(mem, PC);
    
    value = interp_inst(inst);

    if (value == ERROR_UNKNOW_OPCODE)
      return ERROR_UNKNOW_OPCODE; 

    if (value != syscall)
      return 0; 
  }
}


int main(int argc, char const *argv[])
{
  show_status();

  if (argc == 3) {
    read_config(argv[1]);
  } else { 
    printf("Moron, more or less arguments!\n");
    return ERROR_INVALID_ARGS;
  }
  
  if (elf_dump(argv[2], &PC, &mem[0], MEMSZ) != 0)
    return ERROR_ELF_DUMP;
 
  SP = MIPS_RESERVE + (MEMSZ - 4);
 
  if (interp() != 0)
    return ERROR_ELF_DUMP;


  show_status();

  return 0;
}
