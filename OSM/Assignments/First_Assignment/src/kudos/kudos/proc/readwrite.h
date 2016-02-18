/*
 * Print
 */

#ifndef KUDOS_PROC_READWRITE_H
#define KUDOS_PROC_READWRITE_H

#define FILEHANDLER_NOT_NULL 1


#include "lib/libc.h"
#include "drivers/gcd.h"

  int syscall_write(int filehandle, void const *buffer, int length);
  int syscall_read(int filehandle, void *buffer, int length);

#endif /* KUDOS_PROC_READWRITE_H */
