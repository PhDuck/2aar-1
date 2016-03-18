#include "lib.h"

int main() {
  int ret = syscall_filecount("[disk]/");
  printf("Filecount: %d\n", ret);
  printf("Creating a new file...\n");
  syscall_create("[disk]/lolFile", 32);

  ret = syscall_filecount("[disk]/");
  printf("Filecount: %d\n", ret);
  printf("Opening newly created file...\n");
  syscall_open("[disk]/lolFile");

  char buff[] = "This is so fun!\n";
  char buffe[20] = "";

  printf("Trying to write to open file...\n");
  ret = syscall_write(4, buff, sizeof(buff) - 1);
  if (ret > 0) {
    printf("Writing to file: %s", buff);
  }

  printf("Creating and opening a new file...\n");
  syscall_create("[disk]/Patrick", 16);
  ret = syscall_filecount("[disk]/");
  printf("Filecount: %d\n", ret);


  syscall_open("[disk]/Patrick");

  printf("Seeking file to beginning\n");
  syscall_seek(4,0);

  printf("Reading from file?\n");
  ret = syscall_read(4, buffe, sizeof(buffe));
  if (ret > 0) {
    printf("yes, we are reading!\n");
    }
  printf("Read from file: %s", buffe);
  printf("Closing file\n");
  syscall_close(4);


  ret = strcmp(buff, buffe);

  printf("Test if written string is the same as the read string: %s\n", ((ret == 0) ? "True" : "False"));

  printf("Deleting file...\n");

  syscall_delete("[disk]/lolFile");
  ret = syscall_filecount("[disk]/");
  printf("Filecount: %d\n", ret);

  syscall_halt();
  return 0;

}