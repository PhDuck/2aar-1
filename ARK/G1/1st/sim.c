#include <stdio.h>
#include "mips32.h"
#include <stdlib.h>
#include <stdint.h>

#define ERROR_INVALID_ARGS 1
#define ERROR_IO_ERROR 2
#define ERROR_READ_CONFIG_STREAM 3

// Static defs
static uint32_t regs[32];
//static uint32_t PC;

int read_config_stream(FILE *stream)
{
 uint32_t v;

 for (int i=0; i <= 7; i++)
   {
    if (fscanf(stream, "%u", &v) != 1)
      return ERROR_READ_CONFIG_STREAM;
    regs[i + 3] = v;  
   }
  
  return 0;  
}


int read_config (const char *path)
{
 FILE *stream = fopen(path, "r");
 
 if (stream == NULL)
   return ERROR_IO_ERROR;
 if (fclose(stream) != 0)
   return ERROR_IO_ERROR;
   
 return 0;
}



int main(int argc, char const *argv[])
{
  //FILE *file;
  //int file;


  if (argc == 3) {
    read_config(argv[1]);
  } else {
    printf("Moron, more or less arguments!\n");
    return ERROR_INVALID_ARGS;
  }
  return 0;
}
