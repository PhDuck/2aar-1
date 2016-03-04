#ifndef USR_SEM_H
#define USR_SEM_H 

#include "kernel/spinlock.h"
#include "kernel/thread.h"
#include "kernel/interrupt.h"
#include "kernel/sleepq.h"

#define MAX_SEM_TABLE 15 //This is a choice by us.
#define ERROR_NO_EMPTY_IN_SEM_TABLE -101
#define ERROR_UNKNOWN_NAME -102
#define SEM_BLOCKED -103
#define ERROR_NEGATIVE_VALUE -104

typedef struct 
{
	spinlock_t slock;
	const char* name;
	int value;
	TID_t thread;
} usr_sem_t;



usr_sem_t* usr_sem_open(const char* name, int value);
int usr_sem_destory(usr_sem_t* sem);
int find_index_from_name(const char* name);
int find_empty_sem_slot();
int usr_sem_vacate(usr_sem_t* sem);

#endif /* USR_SEM_H */