/* filozofi.c - sprjecavanje deadlocka pomocu monitora (problem pet filozofa)
 * Labos iz Operacijskih Sustava
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM	5

pthread_cond_t table_free_cv, sticks_cv[NUM];
pthread_mutex_t monitor = PTHREAD_MUTEX_INITIALIZER;

int is_eating[NUM] = {0};
int table_free = 2, sticks[NUM] = {1, 1, 1, 1, 1};

void print_status ()
{
	int i;

	for (i=0 ; i<NUM ; ++i)
		if (is_eating[i])
			printf ("jede\t");
		else
			printf ("misli\t");

	printf ("\n");
}

void enter_monitor (int i)
{
	pthread_mutex_lock (&monitor);
	
	/* cekaj dok za stolom ne bude samo jedan filozof */
	while (table_free == 0)
		pthread_cond_wait (&table_free_cv, &monitor);
	
	--table_free;
	
	/* cekaj na lijevi i desni stapic */
	while (sticks[i] == 0)
		pthread_cond_wait (&sticks_cv[i], &monitor);
	
	--sticks[i];
	
	while (sticks[(i + 1) % NUM] == 0)
		pthread_cond_wait (&sticks_cv[(i + 1) % NUM], &monitor);
	
	--sticks[(i + 1) % NUM];
	
	/* jedi jednu sekundu, ispisi status */
	is_eating[i] = 1;
	
	pthread_mutex_unlock (&monitor);
}

void leave_monitor (int i)
{
	pthread_mutex_lock (&monitor);

	is_eating[i] = 0;
	sticks[i] = sticks [(i + 1) % NUM] = 1;
	++table_free;

	pthread_cond_broadcast (&sticks_cv[i]);
	pthread_cond_broadcast (&sticks_cv[(i + 1) % NUM]);
	pthread_cond_broadcast (&table_free_cv);
				
	pthread_mutex_unlock (&monitor);
}

void *philosoph (void *param)
{
	int i = *((int *) param);
	
	srand ((unsigned) time (NULL));
	
	while (1) {
		sleep (1 + rand() % 2);
		enter_monitor (i);
		
		print_status ();
		
		sleep (1 + rand() % 1);
		leave_monitor (i);
	}
}

int main ()
{
	int i, philosoph_id[NUM];
	pthread_t thread_id[NUM];

	/* inicijaliziraj cond varijable */
	pthread_cond_init (&table_free_cv, NULL);
	
	for (i=0 ; i<NUM ; ++i)
		pthread_cond_init (&sticks_cv[i], NULL);
	
	printf ("Fil #1\tFil #2\tFil #3\tFil #4\tFil #5\n");
	printf ("----------------------------------------\n");
	
	/* kreiraj niti */
	for (i=0 ; i<NUM ; ++i) {
		philosoph_id[i] = i;
		pthread_create (&thread_id[i], NULL, philosoph, &philosoph_id[i]);
	}
	
	/* cekaj na sve niti */
	for (i=0 ; i<NUM ; ++i)
		pthread_join (thread_id[i], NULL);

	return EXIT_SUCCESS;
}
