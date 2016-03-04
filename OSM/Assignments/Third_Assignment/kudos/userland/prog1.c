#include "lib.h"

int main() {
	
	usr_sem_t* sem = syscall_usr_sem_open("S1", 0);

	syscall_spawn("[disk]prog2.mips32", NULL);

	printf("Test (1) %d\n", 101);

	syscall_usr_sem_procure(sem);

	printf("Bjon (3)%d\n", 101);

	syscall_halt();
	return 0;
}