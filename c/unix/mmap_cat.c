/* mmap_cat.c - emulate basic cat(1) functionality with mmaped files
 * 
 * (C) 2010 Igor Pozgaj <ipozgaj@gmail.com>
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	char **filename;

	if (argc == 1) {
		printf("Usage mmap_cat [file]...\n");
		return EXIT_SUCCESS;
	}

	for (filename = argv, filename++ ; *filename != NULL ; filename++) {
		int fd, length;
		char *buffer;
		struct stat fs;

		if ((fd = open(*filename, O_RDONLY)) == -1) {
			perror(*filename);
			continue;
		}

		fstat(fd, &fs);
		length = fs.st_size;

		if ((buffer = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
			perror("mmap");
			continue;
		}

		write(STDIN_FILENO, buffer, length);

		close(fd);
	}

	return EXIT_SUCCESS;
}
