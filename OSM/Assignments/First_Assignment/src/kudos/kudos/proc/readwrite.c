
#include "readwrite.h"
#include "kernel/assert.h"
#include "drivers/device.h"
#include "drivers/gcd.h"

int syscall_write(int filehandler, void const *buffer, int length) {
  device_t *dev;
  gcd_t *gcd;

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

  /* Find system console (first tty) */
  dev = device_get(TYPECODE_TTY, filehandler);
  KERNEL_ASSERT(dev != NULL);


  gcd = (gcd_t *)dev->generic_device;
  KERNEL_ASSERT(gcd != NULL);

  KERNEL_ASSERT(length <= 0);

  gcd -> read(gcd, buffer, length);

  return 0;
}
