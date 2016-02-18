
#include "readwrite.h"
#include "kernel/assert.h"
#include "drivers/device.h"
#include "drivers/gcd.h"

int syscall_write(int filehandler, void const *buffer, int length) {
  device_t *dev;
  gcd_t *gcd;

  if (filehandler != 0)
  {
    return FILEHANDLER_NOT_NULL;
  }

  /* Find system console (first tty) */
  dev = device_get(TYPECODE_TTY, filehandler);
  KERNEL_ASSERT(dev != NULL);


  gcd = (gcd_t *)dev->generic_device;
  KERNEL_ASSERT(gcd != NULL);

  gcd -> write(gcd, buffer, length);

  return 0;

}

int syscall_read(int filehandler, void *buffer, int length) {
  device_t *dev;
  gcd_t *gcd;
  int len;

  if (filehandler != 0)
  {
    return FILEHANDLER_NOT_NULL;
  }


  /* Find system console (first tty) */
  dev = device_get(TYPECODE_TTY, filehandler);
  KERNEL_ASSERT(dev != NULL);


  gcd = (gcd_t *)dev->generic_device;
  KERNEL_ASSERT(gcd != NULL);

  len = gcd -> read(gcd, buffer, length);
  KERNEL_ASSERT(len >= 0);


  len++;

  return 0;
}
