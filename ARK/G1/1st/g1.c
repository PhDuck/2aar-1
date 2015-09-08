#include <stdio.h>
#include "mips32.h"
#include <stdlib.h>

#define ERROR_INVALID_ARGS


int read_config (const char *path)
{
  int c;
  FILE *fdesc = fopen(path, "r");
  printf("%s", path);
  while (1)
  {
    c =fgetc(fdesc);
    if (feof(fdesc))
    {
      break;
    }
    printf("%c", c);
  }
  //fprint(fdesc, "%d\n", );
  fclose(fdesc);
  return 0;
}



int main(int argc, char const *argv[])
{
  //FILE *file;
  int file;


  if (argc == 3) {
    file = read_config(argv[1]);
  } else {
    exit(1);
  }
  return 0;
}
