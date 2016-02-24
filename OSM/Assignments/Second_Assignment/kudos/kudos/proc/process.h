/*
 * Process startup.
 */

#ifndef KUDOS_PROC_PROCESS
#define KUDOS_PROC_PROCESS

#include "lib/types.h"
#include "vm/memory.h"
#include <cswitch.h>

#define PROCESS_PTABLE_FULL  -1
#define PROCESS_ILLEGAL_JOIN -2

#define PROCESS_MAX_FILELENGTH 256
#define PROCESS_MAX_PROCESSES  128
#define PROCESS_MAX_FILES      10
#define NO_FREE_PROCESS       -3
#define UNKNOWN_PID           -4

typedef int process_id_t;

typedef enum {
    PROCESS_FREE,
    PROCESS_RUNNING,
    PROCESS_READY,
    PROCESS_SLEEPING,
    PROCESS_EMBRYO,
    PROCESS_ZOMBIE
} process_state_t;



typedef struct {
  /*Penis ID*/
  process_id_t pid;

  // Entry point of memory
  //char *men;

    //ret = setup_new_process(my_thread, executable, argv,
    //                      &entry_point, &stack_top);

  //virtaddr_t

  virtaddr_t stack_top;
  virtaddr_t entry_point;

  context_t user_context;

  /* process state */
  process_state_t state;

} process_control_block_t;

void process_start(const char *executable, const char **argv);



#endif