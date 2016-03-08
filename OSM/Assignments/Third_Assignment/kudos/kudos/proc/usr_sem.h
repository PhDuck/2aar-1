#ifndef USR_SEM_H
#define USR_SEM_H 

#include "kernel/spinlock.h"
#include "kernel/thread.h"
#include "kernel/interrupt.h"
#include "kernel/sleepq.h"
#include "kernel/semaphore.h"

#define MAX_SEM_TABLE 15 //This is a choice by us.
#define NAME_LENGTH 64
#define ERROR_NO_EMPTY_IN_SEM_TABLE -101
#define ERROR_UNKNOWN_NAME -102
#define SEM_BLOCKED -103
#define ERROR_NEGATIVE_VALUE -104
#define ERROR_SEM_NOT_IN_USE -105
#define ERROR_INVALID_POINTER -106

typedef struct 
{
	spinlock_t slock;
	semaphore_t* sem;
	int status; // status 0: free, status 1: taken
	char* name;
	int value;
} usr_sem_t;



usr_sem_t* usr_sem_open(const char* name, int value);
int usr_sem_destory(usr_sem_t* sem);
int find_index_from_name(const char* name);
int find_empty_sem_slot();
int usr_sem_vacate(usr_sem_t* sem);
int usr_sem_procure(usr_sem_t* sem);

#endif /* USR_SEM_H */