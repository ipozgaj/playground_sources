/* shm.c - IPC using shared memory
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{
	char *shared_memory;
	int segment_id, segment_size;
	struct shmid_ds shmbuffer;

	/* we will request 1kb of shared memory */
	const int shared_segment_size = 0x400;

	/* alocate shared memory segment */
	segment_id = shmget(IPC_PRIVATE, shared_segment_size, 
					IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	/* atach segment to process */
	shared_memory = (char *) shmat(segment_id, NULL, 0);
	printf("Shared memory attached at address %p\n", shared_memory);

	/* get shared memory size (it won't be neccessary 1k as we requested) */
	shmctl(segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;

	printf("Requested shared memory size: %d\n", shared_segment_size);
	printf("Alocated  shared memory size: %d\n", segment_size);
	printf("Page size: %d\n", getpagesize());

	/* verify that shared memory segment is correctly attached and writeable */
	sprintf(shared_memory, "Hello world.\n");
	
	/* detach shared memory segment */
	shmdt(shared_memory);

	/* destory shared memory segment */
	shmctl(segment_id, IPC_RMID, 0);

	return EXIT_SUCCESS;
}
