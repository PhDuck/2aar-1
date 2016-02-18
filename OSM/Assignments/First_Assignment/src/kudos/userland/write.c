/*
 * Print what is read from user input.
*/

#include "lib.h"

int main(void) {
  char buffer[100];

  syscall_read(0, &buffer, 100);
  syscall_write(0, &buffer, 100);
  syscall_halt();


  return 0;
}
