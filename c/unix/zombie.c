/* zombie.c - example on creating a zombie process
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid = fork();

	if (pid) {
		/* Parent */
		sleep(30);
	} else {
		/* Child process. Exit normally, so exit status will "hang" as parent is
		sleeping and it's not able to collect our exit status. You may see that
		child is in "zombie" state with ps command */
		exit(0);
	}
		
	return 0;
}
