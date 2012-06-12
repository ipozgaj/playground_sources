/* fifo.c - FIFO (named pipe) creation
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	int fd;

	fd = mkfifo("fifo", S_IWUSR | S_IRUSR);
		
	return 0;
}
