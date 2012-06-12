/* fork.c - creation of new process with fork() system call
 *
 * (C) 2003 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();

	if (pid) {	
		printf("Parent process, PID = %d, created child with PID = %d\n", getpid(), pid);
	} else {
		printf("Child process, PID = %d\n", getpid());
	}

	return 0;
}
