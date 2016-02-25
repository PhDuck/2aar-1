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
#define NO_RUNNING_PROCESS    -5

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

  int retval;
  virtaddr_t stack_top;
  virtaddr_t entry_point;

  context_t user_context;

  /* process state */
  process_state_t state;

} process_control_block_t;

process_id_t process_spawn(const char *executable, const char **argv);

void process_run_thread(uint32_t pid);

int pid_to_index(process_id_t pid);

process_id_t get_free_pid();

void process_init(void)

#endif
