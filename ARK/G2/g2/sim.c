#include "sim.h"

// Static variable declarations.
static uint32_t PC;
static size_t instr_cnt;
static size_t cycles;
static uint32_t regs[32];
static unsigned char mem[MEMSZ];

// Structure declarations
struct preg_if_id {
  uint32_t inst;
  uint32_t next_pc;
};
static struct preg_if_id if_id;

struct preg_id_ex {
  bool mem_read;
  bool mem_write;
  bool reg_write;
  bool alu_src;
  bool mem_to_reg;
  bool branch;
  bool jump;
  uint32_t funct;
  uint32_t rt;
  uint32_t rs; //Hazard
  uint32_t rt_value;
  uint32_t rs_value;
  uint32_t sign_ext_imm;
  uint32_t reg_dst;
  uint32_t next_pc;
  uint32_t jump_target;
};
static struct preg_id_ex id_ex;

struct preg_ex_mem
{
  bool mem_read;
  bool mem_write;
  bool reg_write;
  bool mem_to_reg;
  bool branch;
  uint32_t rt;
  uint32_t rt_value;
  uint32_t alu_res;
  uint32_t reg_dst;
  uint32_t branch_target;
};
static struct preg_ex_mem ex_mem;

struct preg_mem_wb
{
  bool reg_write;
  bool mem_to_reg;
  uint32_t alu_res;
  uint32_t read_data;
  uint32_t rt;
  uint32_t reg_dst;
};
static struct preg_mem_wb mem_wb;

// Debug function
void dump_pregs() {
  printf("if_id:\n");
  printf("PC: %x\n", PC);
  printf("inst: %x\n\n", if_id.inst);

  printf("id_ex:\n");
  printf("mem_read: %x\n", id_ex.mem_read);
  printf("mem_write: %x\n", id_ex.mem_write);
  printf("reg_write: %x\n", id_ex.reg_write);
  printf("rt: %x\n", id_ex.rt);
  printf("rs_value: %x\n", id_ex.rs_value);
  printf("rt_value: %x\n", id_ex.rt_value);
  printf("sign_ext_imm: %x\n", id_ex.sign_ext_imm);
  printf("funct: %x\n\n", id_ex.funct);

  printf("em_mem:\n");
  printf("ex_mem.mem_read: %x\n", ex_mem.mem_read);
  printf("ex_mem.mem_write: %x\n", ex_mem.mem_write);
  printf("ex_mem.reg_write: %x\n", ex_mem.reg_write);
  printf("ex_mem.rt: %x\n", ex_mem.rt);
  printf("ex_mem.rt_value: %x\n", ex_mem.rt_value);
  printf("ex_mem.alu_res: %x\n\n", ex_mem.alu_res);

  printf("mem_wb:\n");
  printf("mem_wb.reg_write: %x\n", mem_wb.reg_write);
  printf("mem_wb.rt: %x\n", mem_wb.rt);
  printf("mem_wb.read_data: %x\n", mem_wb.read_data);
}

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

void interp_wb()
{
  if (mem_wb.reg_write || mem_wb.reg_dst == 0)
    {
      if (mem_wb.mem_to_reg)
       {
         regs[mem_wb.reg_dst] = mem_wb.read_data;
       } else if (!mem_wb.mem_to_reg)
       {
         regs[mem_wb.reg_dst] = mem_wb.alu_res;
       }
    }
}

int alu()
{
  uint32_t second_operand;
  if (id_ex.alu_src)
  {
    second_operand = id_ex.sign_ext_imm;
  } else
  {
    second_operand = id_ex.rt_value;
  }

  switch(id_ex.funct)
  {
    case FUNCT_JR:
      break;
    case FUNCT_ADD:
      ex_mem.alu_res = second_operand + id_ex.rs_value;
      break;
    case FUNCT_SUB:
      ex_mem.alu_res = id_ex.rs_value - second_operand;
      break;
    case FUNCT_AND:
      ex_mem.alu_res = id_ex.rs_value & second_operand;
      break;
    case FUNCT_OR:
      ex_mem.alu_res = id_ex.rs_value | second_operand;
      break;
    case FUNCT_NOR:
      ex_mem.alu_res = ~(id_ex.rs_value | second_operand);
      break;
    case FUNCT_SLT:
      ex_mem.alu_res = ((int32_t) id_ex.rs_value < (int32_t) second_operand) ? 1 : 0;
      break;
    case FUNCT_SYSCALL:
      return SAW_SYSCALL;
    case 0:
      break;
    default:
      return ERROR_UNKNOWN_FUNCT;
  }
  return 0;
}

void interp_mem()
{
  mem_wb.reg_write  = ex_mem.reg_write;
  mem_wb.mem_to_reg = ex_mem.mem_to_reg;
  mem_wb.reg_dst    = ex_mem.reg_dst;
  mem_wb.alu_res    = ex_mem.alu_res;
  mem_wb.rt         = ex_mem.rt;

  if (ex_mem.mem_read)
    {
      mem_wb.read_data = GET_BIGWORD(mem, ex_mem.alu_res);
    }
  if (ex_mem.mem_write)
  {
    SET_BIGWORD(mem, ex_mem.alu_res, ex_mem.rt_value);
  }
}


int interp_ex()
{
  ex_mem.rt            = id_ex.rt;
  ex_mem.rt_value      = id_ex.rt_value;
  ex_mem.mem_read      = id_ex.mem_read;
  ex_mem.mem_write     = id_ex.mem_write;
  ex_mem.reg_write     = id_ex.reg_write;
  ex_mem.mem_to_reg    = id_ex.mem_to_reg;
  ex_mem.reg_dst       = id_ex.reg_dst;
  ex_mem.branch_target = id_ex.next_pc + (id_ex.sign_ext_imm << 2);
  ex_mem.branch        = id_ex.branch;

  if (alu() != 0){
    return ERROR_UNKNOWN_FUNCT;
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


int interp_control(){

  if (if_id.inst == 0){
    id_ex.mem_write = false;
    id_ex.mem_read  = false;
    id_ex.reg_write = false;
    id_ex.alu_src   = false;
    id_ex.branch    = false;
    id_ex.jump      = false;
    id_ex.funct     = GET_FUNCT(if_id.inst);
    return 0;
  }

  switch (GET_OPCODE(if_id.inst)){
  case OPCODE_R:
    id_ex.mem_to_reg = false;
    id_ex.mem_read   = false;
    id_ex.mem_write  = false;
    id_ex.alu_src    = false;
    id_ex.branch     = false;
    id_ex.jump       = false;
    id_ex.reg_dst    = GET_RD(if_id.inst);
    id_ex.reg_write  = true;
    id_ex.funct      = GET_FUNCT(if_id.inst);
    if (id_ex.funct == FUNCT_JR) // If we find JR instruction
      {
        id_ex.jump        = true;
        id_ex.mem_write   = false;
        id_ex.mem_read    = false;
        id_ex.reg_write   = false;
        id_ex.branch      = false;
        id_ex.jump_target = id_ex.rs_value;
      }
    return 0;
  case OPCODE_J:
    id_ex.jump        = true;
    id_ex.mem_write   = false;
    id_ex.mem_read    = false;
    id_ex.reg_write   = false;
    id_ex.branch      = false;
    id_ex.jump_target = (MS_4B & if_id.next_pc) | (GET_ADDRESS(if_id.inst) << 2);

    break;
  case OPCODE_JAL:
    id_ex.jump        = true;
    id_ex.mem_write   = false;
    id_ex.mem_read    = false;
    id_ex.reg_write   = true;
    id_ex.branch      = false;
    id_ex.funct       = FUNCT_ADD;
    id_ex.rs_value    = PC;
    id_ex.rt_value    = 0;
    id_ex.reg_dst     = 31; // RA register index value
    id_ex.jump_target = (MS_4B & if_id.next_pc) | (GET_ADDRESS(if_id.inst) << 2);

    break;
  case OPCODE_BEQ:
    id_ex.branch    = true;
    id_ex.reg_write = false;
    id_ex.mem_read  = false;
    id_ex.mem_write = false;
    id_ex.jump      = false;
    id_ex.alu_src   = false;
    id_ex.funct     = FUNCT_SUB;
    break;

  case OPCODE_BNE:
    id_ex.branch     = true;
    id_ex.reg_write  = false;
    id_ex.mem_write  = false;
    id_ex.mem_read   = false;
    id_ex.jump       = false;
    id_ex.alu_src    = false;
    id_ex.funct      = FUNCT_SUB;
    break;

  case OPCODE_LW:
    id_ex.alu_src    = true;
    id_ex.mem_read   = true;
    id_ex.reg_write  = true;
    id_ex.mem_to_reg = true;
    id_ex.mem_write  = false;
    id_ex.jump       = false;
    id_ex.branch     = false;
    id_ex.funct      = FUNCT_ADD;
    id_ex.reg_dst    = GET_RT(if_id.inst);

    break;
  case OPCODE_SW:
    id_ex.mem_write  = true;
    id_ex.alu_src    = true;
    id_ex.branch     = false;
    id_ex.jump       = false;
    id_ex.mem_read   = false;
    id_ex.reg_write  = false;
    id_ex.mem_to_reg = false;
    id_ex.funct      = FUNCT_ADD;

    break;
  default:
    return ERROR_UNKNOWN_OPCODE;
    break;
  }
  return 0;

}

int interp_id() {
  id_ex.rs_value     = regs[GET_RS(if_id.inst)];
  id_ex.rt_value     = regs[GET_RT(if_id.inst)];
  id_ex.rt           = GET_RT(if_id.inst);
  id_ex.rs           = GET_RS(if_id.inst); //Hazard
  id_ex.sign_ext_imm = SIGN_EXTEND(GET_IMM(if_id.inst));
  id_ex.next_pc      = if_id.next_pc;

  int result_interp_control = interp_control();

  if (result_interp_control == ERROR_UNKNOWN_OPCODE){
    return ERROR_INTERP_CONTROL_FAILED;
  }
  if (result_interp_control == syscall)
  {
    return SAW_SYSCALL;
  }
  return 0;
  }

void interp_if(){
  if_id.inst = GET_BIGWORD(mem, PC);
  PC += 4;
  if_id.next_pc = PC;
  instr_cnt++;
}

//Hazard
void forward() {
  if (ex_mem.reg_write && (ex_mem.reg_dst != id_ex.rs || ex_mem.reg_dst == id_ex.rt))
    {
      if (ex_mem.reg_dst != 0)
        {
          id_ex.rs_value = ex_mem.alu_res;  
          id_ex.rt_value = ex_mem.alu_res;
        }
    }
}


int cycle(){
  // If debug is needed, uncomment following, which will allow step-by-step running.
  //dump_pregs();
  //getchar();

  int interp_id_result;
  interp_wb();
  interp_mem();
  if (interp_ex() == ERROR_UNKNOWN_FUNCT){
    return ERROR_INTERP_EX_FAILED;
  }

  interp_id_result = interp_id();

  if (interp_id_result == ERROR_UNKNOWN_OPCODE){
    return ERROR_INTERP_ID_FAILED;
  }
  if (interp_id_result == SAW_SYSCALL){
    return SAW_SYSCALL;
  }
  interp_if();

  // Branch detection
  if (ex_mem.branch && ex_mem.alu_res == 0)
  {
    PC = ex_mem.branch_target;
    if_id.inst = 0;
    instr_cnt--;
  }
  if (ex_mem.branch && ex_mem.alu_res != 0)
   {
     PC = ex_mem.branch_target;
     if_id.inst = 0;
     instr_cnt--;
   }

  if (id_ex.jump)
  {
    PC = id_ex.jump_target;
  }
  forward();
  return 0;
}


int interp()
{
  while(1)
  {
    int return_cycle;

    return_cycle = cycle();

    if (return_cycle == SAW_SYSCALL)
    {
      return 0;
    } else if ( return_cycle != 0)
    {
      break;
    }
    cycles++;
  }
  return 0;
}

int main(int argc, char const *argv[])
{
  int interp_result;
  int read_config_result;

  if (argc == 3)
  {
    read_config_result = read_config(argv[1]);
    if (read_config_result != 0){
      return read_config_result;
    }
  } else {
    printf("Three arguments are needed, one file register values and one compiled asm file\n");
    return ERROR_INVALID_ARGS;
  }
  if (elf_dump(argv[2], &PC, &mem[0], MEMSZ) != 0) {
    return ERROR_ELF_DUMP;
  }
  SP = MIPS_RESERVE + MEMSZ;

  interp_result = interp();

  if (interp_result != 0) {
    return interp_result;
  }

  show_status();
  return 0;
}

