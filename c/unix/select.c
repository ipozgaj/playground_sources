#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define SELECT_TIMEOUT 5

int main()
{
	fd_set readfds, writefds;
	struct timeval tv;

	/* check stding for input */
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	/* check stdout for writting */
	FD_ZERO(&writefds);
	FD_SET(STDOUT_FILENO, &writefds);

	tv.tv_sec = SELECT_TIMEOUT;
	tv.tv_usec = 0;

	if (select(STDOUT_FILENO + 1, &readfds, &writefds, NULL, &tv) == -1) {
		perror("select");
		return EXIT_FAILURE;
	}

	if (FD_ISSET(STDIN_FILENO, &readfds))
		printf("STDIN is ready for reading\n");

	if (FD_ISSET(STDOUT_FILENO, &writefds))
		printf("STDOUT is ready for writing\n");

	return EXIT_SUCCESS;
}
