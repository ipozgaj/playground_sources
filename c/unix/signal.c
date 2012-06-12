/* signals.c - custom signal handlers
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler()
{
	fprintf(stderr, "Received: SIGINT(2)\n");
}

void sigquit_handler()
{
	fprintf(stderr, "Received: SIGQUIT(3)\n");
}

void sigterm_handler()
{
	fprintf(stderr, "Received: SIGTERM(15)\n");
}

void sigalrm_handler()
{
	fprintf(stderr, "Received: SIGALRM(14)\n");
	exit(0);
}

int main()
{
	struct sigaction sa;
	
	memset(&sa, 0, sizeof (struct sigaction));
	
	sa.sa_handler = sigalrm_handler;	
	sigaction(SIGALRM, &sa, NULL);

	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	
	sa.sa_handler = sigquit_handler;
	sigaction(SIGQUIT, &sa, NULL);
	
	sa.sa_handler = sigterm_handler;
	sigaction(SIGTERM, &sa, NULL);

	/* send SIGALRM in 10 sec */
	alarm(10);
	
	/* Run in infinite loop, as program will be stopped by SIGALRM handler */
	for (;;);
	
	return EXIT_SUCCESS;
}
