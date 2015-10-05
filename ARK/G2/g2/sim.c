#include "sim.h" 

// Static variable declarations.
static uint32_t PC;
static size_t instr_cnt;
static size_t cycles;
static uint32_t regs[32];
static unsigned char mem[MEMSZ];

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
  uint32_t funct;
  uint32_t rt;
  uint32_t rt_value;
  uint32_t rs_value;
  uint32_t sign_ext_imm;
  uint32_t reg_dst;
  uint32_t next_pc;
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

void dump_pregs() {
  printf("if_id:\n");
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
         printf("mem_wb.read_data: %x\n", mem_wb.read_data);
         printf("mem_wb.reg_dst: %x\n", mem_wb.reg_dst);
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
    second_operand = id_ex.rt_value;
  } else if (!id_ex.alu_src)
  {
    second_operand = id_ex.sign_ext_imm;
  }

  switch(id_ex.funct)
  {
    case FUNCT_ADD:
      printf("ADD\n");
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
      printf("nop\n");
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
      printf("ex_mem.alu_res: %x\n", mem_wb.alu_res);
      mem_wb.read_data = GET_BIGWORD(mem, ex_mem.alu_res + 1);
    }
  if (ex_mem.mem_write)
  {
    printf("ex_mem.rt_value: %x\n", ex_mem.rt_value);
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
  //uint32_t result;

  if (if_id.inst == 0){
    id_ex.mem_write = false;
    id_ex.mem_read  = false;
    id_ex.reg_write = false;
    id_ex.alu_src   = false;
    id_ex.branch    = false;
    id_ex.funct     = GET_FUNCT(if_id.inst);
    return 0;
  }

  switch (GET_OPCODE(if_id.inst)){
  case OPCODE_R:
    id_ex.mem_to_reg = false;
    id_ex.mem_read   = false;
    id_ex.mem_write  = false;
    id_ex.alu_src    = true;
    id_ex.branch     = false;
    id_ex.reg_dst    = GET_RD(if_id.inst);
    id_ex.reg_write  = true;
    id_ex.funct      = GET_FUNCT(if_id.inst);
    //result = intep_r(if_id.inst);
    /**
    if (result == syscall) {
      return syscall;
    }
    **/
    return 0;
  case OPCODE_BEQ:
    id_ex.branch    = true;
    id_ex.reg_write = false;
    id_ex.mem_read  = false;
    id_ex.mem_write = false;
    id_ex.alu_src   = true;
    id_ex.funct     = FUNCT_SUB;

    break;
  case OPCODE_LW:
    printf("LW!\n");
    id_ex.alu_src    = true;
    id_ex.mem_read   = true;
    id_ex.reg_write  = true;
    id_ex.mem_to_reg = true;
    id_ex.mem_write  = false;
    id_ex.branch     = false;
    id_ex.funct      = FUNCT_ADD;
    id_ex.reg_dst    = GET_RT(if_id.inst);

    break;
  case OPCODE_SW:
    printf("SW\n");
    id_ex.mem_write  = true;
    id_ex.alu_src    = true;
    id_ex.branch     = false;
    id_ex.mem_read   = false;
    id_ex.reg_write  = false;
    id_ex.mem_to_reg = false;
    id_ex.funct      = FUNCT_ADD;

    break;
  case OPCODE_LUI:
    printf("LUI\n");//Ikke sikker
    //rt = if_id.sign_ext_imm << 16;
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
  id_ex.sign_ext_imm = SIGN_EXTEND(GET_IMM(if_id.inst));
  int result         = interp_control();
  id_ex.next_pc      = if_id.next_pc;


  if (result == ERROR_UNKNOWN_OPCODE){
    return ERROR_INTERP_CONTROL_FAILED;
  } else if (result == syscall)
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

int cycle(){
  //dump_pregs();
  int result;
  interp_wb();
  interp_mem(); 
  if (interp_ex() == ERROR_UNKNOWN_FUNCT){
    return ERROR_INTERP_EX_FAILED;
  }

  result = interp_id();

  if (result == ERROR_UNKNOWN_OPCODE){
    return ERROR_INTERP_ID_FAILED;
  }
  if (result == SAW_SYSCALL){
    return SAW_SYSCALL;
  }
  interp_if();

  //printf("%s\n", ex_mem.branch ? "true" : "false");
  if (ex_mem.branch && ex_mem.alu_res == 0) 
  {
    PC = ex_mem.branch_target;
    if_id.inst = 0;
    instr_cnt--;
  }
  return 0;
}

int interp()
{
  while(1)
  {
    int return_cycle;

    return_cycle = cycle();
    //show_status();
    //dump_pregs();
    if (return_cycle == SAW_SYSCALL)
    {
      return 0;
    } else if ( return_cycle != 0)
    {
      break;
    }
    cycles++;
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
  SP = MIPS_RESERVE + MEMSZ;

  if (interp() != 0) {
    return ERROR_ELF_DUMP;
  }
  show_status();
  return 0;
}

