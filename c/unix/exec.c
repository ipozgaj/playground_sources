/* exec.c - fork a new process and replace it with a new program
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	char *program = "ps";
	char *arg_list[] = {"ps", "-f", NULL};
  
  	/* fork a new process */
	pid = fork();

	if (pid) {
		/* Parent process, wait for child to finish */
		wait(NULL);
	} else {
		/* replace running process with a new program instance */
		execvp(program, arg_list);
    
		/* This should never print if exec was successfull */
		fprintf(stderr, "Error while calling execvp\n");
	}

	return 0;
}
