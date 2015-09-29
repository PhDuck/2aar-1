#include "sim.h"

// Static variable declarations.
static uint32_t PC;
static size_t instr_cnt;
static size_t cycles;
static uint32_t regs[32];
static unsigned char mem[MEMSZ];

struct preg_if_id {
  uint32_t inst;
  uint8_t rt;
  uint8_t rs_value;
  uint8_t rt_value;
  uint16_t sign_ext_imm;
 
};
static struct preg_if_id if_id;

struct preg_id_ex {
  bool mem_read;
  bool mem_write;
  bool reg_write;
  uint8_t funct;
  uint8_t rs;
  uint8_t rt;
  uint8_t rt_value;
  uint8_t rs_value;
  uint16_t sign_ext_imm;
};
static struct preg_id_ex id_ex;
/**
static struct if_id{};
static struct id_ex{};
static struct ex_mem{};
static struct mem_wb{};
**/
struct preg_ex_mem
{
  bool mem_read;
  bool mem_write;
  bool reg_write;
  uint8_t rt;
  uint8_t rt_value;
  uint32_t alu_res;
};
static struct preg_ex_mem ex_mem;

int show_status()
{
  printf("Executed %zu instrucitons(s).\n", instr_cnt);
  printf("%zu cycle(s) elapses\n", cycles);
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

int alu()
{
  switch(id_ex.funct)
  {
    case FUNCT_ADD:
    printf("ADD\n");
    ex_mem.alu_res = id_ex.sign_ext_imm + id_ex.rs_value;
    break;
    case 0:
    printf("nop\n");
    break;
    default:
    return ERROR_UNKNOW_FUNCT;
  }
  return 0;
}

int interp_ex()
{
  ex_mem.rt = id_ex.rt;
  ex_mem.rt_value = id_ex.rt_value;
  id_ex.sign_ext_imm = if_id.sign_ext_imm;

  if (alu() != 0){
    return ERROR_UNKNOW_FUNCT;
  }
  return 0;
}

int read_config_stream(FILE *stream)
{
  uint32_t v;
  // Only input data into the regs from 8 - 15, which are Temp Registers.
  for (int i = 8; i <= 15; i++)
    {
    if (fscanf(stream, "%u", &v) != 1) {
      return ERROR_READ_CONFIG_STREAM;
    }
    regs[i] = v;
    }
  return 0;
}


int read_config (const char *path)
{
  FILE *stream = fopen(path, "r");

  if (stream == NULL) {
    return ERROR_IO_ERROR;
  } else if (read_config_stream(stream) != 0) {
    return ERROR_READ_CONFIG_STREAM;
  }
  if (fclose(stream) != 0) {
    return ERROR_IO_ERROR;
  }
  return 0;
}

/**
int interp_mem(){
  return 0;
  }

int interp_wb(){
  return 0;
  }
**/

int intep_r(uint32_t inst)
{
  uint32_t rs    = regs[GET_RS(inst)];
  uint32_t rt    = regs[GET_RT(inst)];
  uint8_t shamt = GET_SHAMT(inst);
  uint8_t funct = GET_FUNCT(inst);
  uint32_t rd   = GET_RD(inst);
  uint32_t *rdd = &regs[rd];
  /*
  uint32_t rs    = regs[GET_RS(if_id.inst)];
  uint32_t rt    = regs[GET_RT(if_id.inst)];
  if_id.rt = GET_RT(if_id.inst);
  if_id.rs_value = regs[rs];
  if_id.rt_value = regs[rt];
  if_id.sign_ext_imm = SIGN_EXTEND(GET_IMM(if_id.inst));
  */

  switch (funct)
  {
  case FUNCT_JR:
    printf("JR\n");
    printf("%x\n", PC);
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
  uint8_t rs         = regs[GET_RS(inst)];
  uint8_t rt         = regs[GET_RT(inst)];
  uint32_t *rtt      = &regs[GET_RT(inst)];
  uint16_t immediate = GET_IMM(inst);
  switch (GET_OPCODE(inst))
  {
  case OPCODE_R:
    result = intep_r(inst);
    if (result == syscall) {
      return syscall;
    }
    break;
  case OPCODE_J:
    PC = (MS_4B & PC) | (GET_ADDRESS(inst)  << 2);
    break;
  case OPCODE_JAL:
    RA = PC + 8;
    PC =(MS_4B & PC) | (GET_ADDRESS(inst)  << 2);
    break;
  case OPCODE_BEQ:
    if (rs == rt) {
      PC = PC + 4 + (SIGN_EXTEND(immediate) << 2);
    }
    break;
  case OPCODE_BNE:
    if (rs != rt) {
      PC = PC + 4 + (SIGN_EXTEND(immediate) << 2);
    }
    break;
  case OPCODE_ADDIU:
    *rtt = rs + SIGN_EXTEND(immediate);
    break;
  case OPCODE_SLTI:
    *rtt =  (rs < SIGN_EXTEND(immediate)) ? 1 : 0;
    break;
  case OPCODE_ANDI:
    *rtt = rs & ZERO_EXTEND(immediate);
    break;
  case OPCODE_ORI:
    *rtt = rs | ZERO_EXTEND(immediate);
    break;
  case OPCODE_LUI:
    *rtt = immediate << 16;
    break;
  case OPCODE_LW:
    *rtt = GET_BIGWORD(mem, rs + SIGN_EXTEND(immediate));
    break;
  case OPCODE_SW:
    SET_BIGWORD(mem, rs + SIGN_EXTEND(immediate), rt)
    break;
  default:
    return ERROR_UNKNOW_OPCODE;
  }
  return interp_not_done;
}

int interp_control(){
  uint32_t result;

  if (if_id.inst == 0){
    return 0;
  }

  switch (GET_OPCODE(if_id.inst)){
  case OPCODE_R:
    result = intep_r(if_id.inst);
    if (result == syscall) {
      return syscall;
    }
  case OPCODE_LW:
    printf("LW!\n");
    id_ex.mem_read = true;
    id_ex.reg_write = true;
    id_ex.funct = FUNCT_ADD;
    // We don't have to define mem_write to false since it's a static struct, and therefore bool = false
    break;
  case OPCODE_SW:
    printf("SW\n");
    id_ex.mem_write = true;
    id_ex.funct = FUNCT_ADD;
    break;
  case OPCODE_LUI:
  printf("LUI\n");//Ikke sikker
  //rt = if_id.sign_ext_imm << 16;

  default:
    return ERROR_UNKNOW_OPCODE;
    break;
  }
  return 0;

}

int interp_id() {
  uint32_t rs    = regs[GET_RS(if_id.inst)];
  uint32_t rt    = regs[GET_RT(if_id.inst)];
  if_id.rt = GET_RT(if_id.inst);
  if_id.rs_value = rs;
  if_id.rt_value = rt;
  if_id.sign_ext_imm = SIGN_EXTEND(GET_IMM(if_id.inst));
  
  if (interp_control() == ERROR_UNKNOW_OPCODE){
    return ERROR_INTERP_CONTROL_FAILED;
  }
  return 0;
  }

void interp_if(){
  if_id.inst = GET_BIGWORD(mem, PC);
  PC += 4;
  instr_cnt++;
}



int cycle(){
  if (interp_ex() == ERROR_UNKNOW_FUNCT){
    return ERROR_INTERP_EX_FAILED;
  }

  if (interp_id() == ERROR_UNKNOW_OPCODE){
    return ERROR_INTERP_ID_FAILED;
  }
  if(interp_id() == syscall){
    return SAW_SYSCALL;
  }
  interp_if();
  return 0;
}

int interp()
{
  while(1)
  {
    int return_cycle;
    cycles++;

    return_cycle = cycle();
    show_status();
    if (return_cycle == SAW_SYSCALL){
      return 0;
      } else if ( return_cycle != 0){
        break;
      }
    
    /**
    instr_cnt++;
    uint32_t value;
    uint32_t inst = GET_BIGWORD(mem, PC);

    value = interp_inst(inst)

    if (value == ERROR_UNKNOW_OPCODE) {
      return ERROR_UNKNOW_OPCODE;
    }
    if (value == syscall) {
      return 0;
    }
    PC = PC + 4;
    **/
  }
  return 0;
}

int main(int argc, char const *argv[])
{
  if (argc == 3) {
    read_config(argv[1]);
  } else {
    printf("Moron, try again with more, or less arguments!\n");
    return ERROR_INVALID_ARGS;
  }
  if (elf_dump(argv[2], &PC, &mem[0], MEMSZ) != 0) {
    return ERROR_ELF_DUMP;
  }
  SP = MIPS_RESERVE + (MEMSZ - 4);

  if (interp() != 0) {
    return ERROR_ELF_DUMP;
  }
  show_status();
  return 0;
}
