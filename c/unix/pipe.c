/* pipe.c - IPC by using pipes
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int writer(const char *msg, int count, FILE *stream)
{
	for (; count > 0 ; count--) {
		fprintf(stream, "%s\n", msg);
		fflush(stream);
		sleep(1);
	}
}

int reader(FILE *stream)
{
	char buffer[1024];

	while (!feof(stream) && !ferror(stream) && fgets(buffer, sizeof (buffer), stream) != NULL)
		fputs(buffer, stdout);
}

int main ()
{
	int fds[2];
	pid_t pid;
			
	pipe(fds);

	pid = fork();

	if (pid == 0) {
		FILE *stream;
		
		close(fds[1]);

		stream = fdopen(fds[0], "r");
		
		reader(stream);
		
		close(fds[0]);
	} else {
		FILE *stream;

		close(fds[0]);

		stream = fdopen(fds[1], "w");
		
		writer("TEST", 6, stream);
		
		close(fds[1]);
	}

	return EXIT_SUCCESS;
}
