/* SV5sem.c - process synchronization with process (SYSV) semaphores
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

/* semaphore union */
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

/* get semaphore ID, allocate a new semaphore if needed */
int allocate_semaphore(key_t key, int sem_flags)
{
	return semget(key, 1, sem_flags);
}

/* deallocate semaphore */
int deallocate_semaphore(int semid)
{
	union semun ignored_argument;
	
	return semctl(semid, 1, IPC_RMID, ignored_argument);
}

/* semaphore initializaion */
int semaphore_initialize(int semid)
{
	union semun arguments;
	unsigned short values[1];

	values[0] = 1;
	arguments.array = values;

	return semctl(semid, 0, SETALL, arguments);
}

/* wait operation - block until semaphore value gets positive, than decrement by one */
int semaphore_wait(int semid)
{
	struct sembuf operations[1];

	/* use first (and only) semaphore */
	operations[0].sem_num = 0;
	operations[0].sem_op = -1;
	operations[0].sem_flg = SEM_UNDO;

	return semop(semid, operations, 1);
}

/* post operation - increment by one */
int semaphore_post(int semid)
{
	struct sembuf operations[1];

	/* use first (and only) semaphore */
	operations[0].sem_num = 0;
	operations[0].sem_op = 1;
	operations[0].sem_flg = SEM_UNDO;

	return semop(semid, operations, 1);
}

int main ()
{
	return EXIT_SUCCESS;
}
