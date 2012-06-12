/* detached.c - creation of detached thread
 * 
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *unused)
{
	return NULL;
}

int main()
{
	pthread_t thread;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
 
 	/* alternative approach is to create an ordinary joinable
	   thread and make it detached with pthread_detach() */
	pthread_create(&thread, &attr, &thread_function, NULL);

	pthread_attr_destroy(&attr);

	return EXIT_SUCCESS;
}
