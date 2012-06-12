#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>

#define POLL_TIMEOUT 5

int main()
{
	struct pollfd fds[2];

	/* check stdin for input */
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	/* check stdout for writting */
	fds[1].fd = STDOUT_FILENO;
	fds[1].events = POLLOUT;

	if (poll(fds, 2, POLL_TIMEOUT * 1000) == -1) {
		perror("poll");
		return EXIT_FAILURE;
	}

	if (fds[0].revents & POLLIN)
		printf("STDIN is ready for reading\n");

	if (fds[1].revents & POLLOUT)
		printf("STDOUT is ready for writing\n");

	return EXIT_SUCCESS;
}
