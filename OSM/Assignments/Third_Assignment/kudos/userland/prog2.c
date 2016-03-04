#include "lib.h"


int main() {
	susr_sem_t* sem = syscall_usr_sem_open("S1", -1);
	printf("Mirza (2)%d", 101);
	syscall_usr_sem_vacate(sem);

	return 0;
}