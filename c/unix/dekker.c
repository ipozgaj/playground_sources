/* dekker.c - iskljucivanje niti Dekkerovim algoritmom
 * Labos iz Operacijskih Sustava
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 *
 * Prevesti sa: gcc -lpthread -D_REENTRANT dekker.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS_NUM 2

int access_right = 0;
int flags[THREADS_NUM] = {0};

void lock_state (int i, int j)
{
	flags[i] = 1;

	while (flags[j]) {
		if (access_right == j) {
			flags[i] = 0;
			
			while (access_right == j);

			flags[i] = 1;
		}
	}
}

void unlock_state (int i, int j)
{
	access_right = j;
	flags[i] = 0;
}

void *thread_function (void *i)
{
	int k, m;

	for (k=1 ; k<=5 ; ++k) {
		lock_state (*((int *) i), 1 - *((int *) i));

		for (m=1 ; m<=5 ; ++m) {
			printf ("%d ", * ((int *) i));
			printf ("%d ", k);
			printf ("%d\n", m);
			sleep (1);
		}

		unlock_state (*((int *) i), 1 - *((int *) i));
	}

	return NULL;
}

int main ()
{
	int i = 0, j = 1;
	pthread_t thread1_id, thread2_id;
	
	if (pthread_create (&thread1_id, NULL, &thread_function, &i)) {
		perror ("pthread_create");
		exit (EXIT_FAILURE);
	}

	if (pthread_create (&thread2_id, NULL, &thread_function, &j)) {
		perror ("pthread_create");
		exit (EXIT_FAILURE);
	}
	
	if (pthread_join (thread1_id, NULL)) {
		perror ("pthread_join");
		exit (EXIT_FAILURE);
	}
	
	if (pthread_join (thread2_id, NULL)) {
		perror ("pthread_join");
		exit (EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
