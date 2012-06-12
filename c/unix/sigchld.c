/* sigchld.c - asynchronous zombie cleaning with SIGCHLD handler
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

sig_atomic_t child_exit_status;

void handler(int signal_number)
{
	int status;

	wait(&status);

	child_exit_status = status;
}

int main()
{
	pid_t pid;
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handler;
	sigaction(SIGCHLD, &sa, NULL);

	pid = fork();

	if (pid) {
		/* Parent, sleep for 30s to make sure child will become zombie */
		sleep(30);
	} else {
		/* Exit immediately. No zombie should be reported within ps command
	       output as it should be collected by signal handler */
		exit(0);
	}
		
	return 0;
}
