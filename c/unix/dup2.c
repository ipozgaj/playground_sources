/* redirect.c - stdout redirection
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	int fds[2];
	pid_t pid;

	pipe(fds);

	pid = fork();

	if (pid == 0) {
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		execlp("sort", "sort", 0);
	} else {
		FILE *stream;

		close (fds[0]);

		stream = fdopen(fds[1], "w");

		fprintf(stream, "This\n");
		fprintf(stream, "should\n");
		fprintf(stream, "be\n");
		fprintf(stream, "sorted\n");

		close(fds[1]);

		waitpid(pid, NULL, 0);
	}
		
	return EXIT_SUCCESS;
}
