#include "usr_sem.h"

static usr_sem_t sem_table[MAX_SEM_TABLE];

static spinlock_t sem_table_slock;

void user_sem_init(void)
{
  spinlock_reset(&sem_table_slock);
  for(int i = 0; i < MAX_SEM_TABLE; i++)
  {
  	sem_table[i].status = 0;
  }
}


int find_empty_sem_slot() {
	for (int i = 0; i < MAX_SEM_TABLE; ++i)
	{
		if (sem_table[i].status == 0)
		{
			return i;
		}
	}
	return ERROR_NO_EMPTY_IN_SEM_TABLE;
}

int find_index_from_name(const char* name) {
	for (int i = 0; i < MAX_SEM_TABLE; ++i)
	{
		if (stringcmp(sem_table[i].name, name))
		{
			return i;
		}
	}
	return ERROR_UNKNOWN_NAME;
}

usr_sem_t* usr_sem_open(const char* name, int value) {
	

	spinlock_acquire(&sem_table_slock);

	if (value >= 0)
	{
		for (int i = 0; i < MAX_SEM_TABLE; ++i)
		{
			if (stringcmp(sem_table[i].name, name)) 
			{
				spinlock_release(&sem_table_slock);
				return NULL;
			}
		}
		int index = find_empty_sem_slot(); 
		sem_table[index].sem = semaphore_create(value);
		sem_table[index].status = 1;
		stringcopy(sem_table[index].name, name);
		sem_table[index].value = value;

		spinlock_release(&sem_table_slock);

		return &sem_table[index];
	}

	if (value < 0 ) 
		{
			for (int i = 0; i < MAX_SEM_TABLE; ++i)
			{
				if (stringcmp(sem_table[i].name, name)) 
				{
					spinlock_release(&sem_table_slock);
					return &sem_table[i]; 
				}
			}
			
		spinlock_release(&sem_table_slock);
		return NULL;
		}

	spinlock_release(&sem_table_slock);
	return NULL;
}

int usr_sem_destory(usr_sem_t* sem) {
	interrupt_status_t intr_status;

	intr_status = _interrupt_disable();
	spinlock_acquire(&sem->slock);
	if (sem -> value < 0)
	{
		spinlock_release(&sem -> slock);	
		_interrupt_set_state(intr_status);
		return SEM_BLOCKED;
	}
	
	semaphore_destroy(sem);
	sem -> status = 0;
	sem -> value = 0;
	
	spinlock_release(&sem -> slock);
	_interrupt_set_state(intr_status);
	return 0;
}

int usr_sem_procure(usr_sem_t* sem) {
  interrupt_status_t intr_status;

  intr_status = _interrupt_disable();
  spinlock_acquire(&sem->slock);
  
  if(sem -> status == 0){
  	return ERROR_SEM_NOT_IN_USE;
  }

  if(sem -> sem -> creator == -1){
  	return ERROR_INVALID_POINTER;
  }

  semaphore_P(sem);
  /*sem->value--;

  while (sem->value < 0) {
    sleepq_add(sem);
    spinlock_release(&sem->slock);
    thread_switch();
    spinlock_acquire(&sem->slock);
  }*/

  spinlock_release(&sem->slock);

  _interrupt_set_state(intr_status);
  return 0;
}

int usr_sem_vacate(usr_sem_t* sem) {
  interrupt_status_t intr_status;
    
  intr_status = _interrupt_disable();
  spinlock_acquire(&sem->slock);

  if(sem -> status == 0){
  	return ERROR_SEM_NOT_IN_USE;
  }

  if(sem -> sem -> creator == -1){
  	return ERROR_INVALID_POINTER;
  }

  semaphore_V(sem);

/*  sem->value++;
  if (sem->value <= 0) {
    sleepq_wake(sem);
  }*/

  spinlock_release(&sem->slock);
  _interrupt_set_state(intr_status);
  return 0;
}
