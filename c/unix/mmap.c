/* memory_mapping.c - IPC with mapped memmory
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

/* get a pseudorandom number withing given interval. use uniform distribuion */
int random_range(int low, int high)
{
	int range = high - low;

	return low + (int) (((double) range) * rand () / (RAND_MAX + 1.0));
}

int main(int argc, char **argv)
{
	int fd;
	void *file_memory;

	if (argc != 2) {
		printf("Output file not specified!\n");
		exit(EXIT_FAILURE);
	}
	
	srand((unsigned) time(NULL));

	/* open file and seek to the end of the file */
	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	
	/* allocate space in file */
	lseek(fd, FILE_LENGTH + 1, SEEK_SET);
	write(fd, "", 1);
	lseek(fd, 0, SEEK_SET);

	file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
	
	close(fd);

	/* write random numbers to memmory */
	sprintf((char *) file_memory, "%d\n", random_range(-100, 100));

	munmap(file_memory, FILE_LENGTH);
	
	return EXIT_SUCCESS;
}

