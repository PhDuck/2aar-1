/*
 * System calls.
 */
#include <cswitch.h>
#include "proc/syscall.h"
#include "kernel/halt.h"
#include "kernel/panic.h"
#include "lib/libc.h"
#include "kernel/assert.h"
#include "vm/memory.h"
#include "drivers/polltty.h"
#include "proc/process.h"
#include "fs/vfs.h"

typedef struct
{
  openfile_t oid;
} IDsMaaan;

static IDsMaaan TableIdsMaaan[PROCESS_MAX_PROCESSES];


int syscall_write(const char *buffer, int length) {
  /* Not a G1 solution! */
  for (int i = 0; i < length; i++, *buffer++) polltty_putchar(*buffer);
  return length;
}

int syscall_read(char *buffer) {
  /* Not a G1 solution! */
  *buffer = polltty_getchar();
  return 1;
}

int syscall_open_helper(char* pathname) {
  int vfsID;
  process_id_t pid;
  vfsID = vfs_open(pathname);
  pid = process_get_current_process();
  TableIdsMaaan[pid].oid = vfsID;
  return vfsID;
}

/**
 * Handle system calls. Interrupts are enabled when this function is
 * called.
 */
uintptr_t syscall_entry(uintptr_t syscall,
                        uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
  arg0 = arg0;
  arg1 = arg1;
  arg2 = arg2;
  /* When a syscall is executed in userland, register a0 contains
   * the number of the syscall. Registers a1, a2 and a3 contain the
   * arguments of the syscall. The userland code expects that after
   * returning from the syscall instruction the return value of the
   * syscall is found in register v0. Before entering this function
   * the userland context has been saved to user_context and after
   * returning from this function the userland context will be
   * restored from user_context.
   */
  switch(syscall) {
  case SYSCALL_HALT:
    halt_kernel();
    break;
  case SYSCALL_READ:
    return syscall_read((void*)arg1);
    break;
  case SYSCALL_WRITE:
    return syscall_write((const void*)arg1, (int)arg2);
    break;
  case SYSCALL_SPAWN:
    return process_spawn((char*) arg0, (char const**) arg1);
    break;
  case SYSCALL_EXIT:
    process_exit((process_id_t) arg0);
    vfs_close(TableIdsMaaan[arg0].oid);
    break;
  case SYSCALL_JOIN:
    return process_join((process_id_t) arg0);
    break;
  case SYSCALL_OPEN:
    return syscall_open_helper((char*) arg0);
    break;
  case SYSCALL_CLOSE:
    return vfs_close((openfile_t) arg0);
    break;
  case SYSCALL_SEEK:
    return vfs_seek((openfile_t) arg0, (int) arg1);
    break;
  case SYSCALL_CREATE:
    return vfs_create((char*) arg0, (int) arg1);
    break;
  case SYSCALL_DELETE:
    return vfs_remove((char*) arg0);
    break;
  case SYSCALL_FILECOUNT:
      return vfs_filecount((char*) arg0);
    break;
  case SYSCALL_FILE:
      return vfs_file((char*) arg0, arg1, (char*) arg2)
    break;
  default:
    KERNEL_PANIC("Unhandled system call\n");
  }

  return 0;
}