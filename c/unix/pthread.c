/* pthread.c - thread creation
 *
 * compile with i -lpthread -D_REENTRANT
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 5

void *thread_function(void *arg)
{
	while(1) {
		printf("Thread: %d (ID = %d)\n", *(int *) arg, pthread_self());
		sleep(1);
	}
}

int main()
{
	int i, id[MAX_THREADS];
	pthread_t threads[MAX_THREADS];

	for (i = 0 ; i < MAX_THREADS ; ++i) {
		id[i] = i;
		pthread_create(&threads[i], NULL, &thread_function, &id[i]);
	}

	/* wait for threads to finish */
	for (i = 0 ; i < MAX_THREADS ; ++i) {
		pthread_join(threads[i], NULL);
	}

	return EXIT_SUCCESS;
}
