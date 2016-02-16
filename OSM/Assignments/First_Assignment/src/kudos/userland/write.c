/*
 * Halt the system from userland.
 */

#include "lib.h"

int main(void) {
  syscall_write(0, "Mizra\n", 10);
  syscall_read(1, "testerino \n", 20);
  syscall_halt();
  return 0;
}
