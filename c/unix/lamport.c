/* lamport.c - iskljucivanje niti Lamportovim algoritmom
 *
 * Labos iz Operacijskih Sustava
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 *
 * Prevesti sa: gcc -lpthread -D_REENTRANT lamport.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int number_of_threads;
int *search, *num;

int get_max_value (int *array)
{
	int i, maxval;

	for (i=0, maxval=0 ; i<number_of_threads ; ++i)
		if (array[i] > array[maxval]) maxval = i;

	return array[maxval];
}

int cmp4 (int a1, int b1, int a2, int b2)
{
	if (a1 == a2) {
		if (b1 > b2)
			return 1;
		else if (b1 < b2)
			return -1;
		else 
			return 0;
	}

	if (a1 > a2) 
		return 1;
	else
		return -1;
}

void lock_state (int i)
{
	int j;
		
	search[i] = 1;
	num[i] = get_max_value (num) + 1;
	search[i] = 0;
	
	for (j=0 ; j<number_of_threads ; ++j) {
		while (search[j]);
		while (num[j] && (cmp4 (num[j], j, num[i], i) == -1));
	}
}

void unlock_state (int i)
{
	num[i] = 0;
}

void *thread_function (void *i)
{
	int k, m;

	for (k=1 ; k<=5 ; ++k) {
		lock_state (*((int *) i));

		for (m=1 ; m<=5 ; ++m) {
			printf ("%d ", 1 + * ((int *) i));
			printf ("%d ", k);
			printf ("%d\n", m);
			sleep (1);
		}

		unlock_state (*((int *) i));
	}

	return NULL;
}

int main (int argc, char **argv)
{
	int i;
	int *params;
	pthread_t *threads;
		
	if (argc != 2) {
		fprintf (stderr, "Koristenje: lamport broj_niti\n");
		exit (EXIT_FAILURE);
	}

	if ((sscanf (argv[1], "%d", &number_of_threads)) != 1) {
		fprintf (stderr, "Koristenje: lamport broj_niti\n");
		exit (EXIT_FAILURE);
	}

	search = calloc (number_of_threads, sizeof (int));
	num = calloc (number_of_threads, sizeof (int));
	threads = malloc (number_of_threads * sizeof (pthread_t));
	params = malloc (number_of_threads * sizeof (int));
	
	if (!search || !num || !threads || !params) {
		fprintf (stderr, "Alokacija memorije nije uspjela\n");
		exit (EXIT_FAILURE);
	}
	
	for (i = 0 ; i < number_of_threads ; ++i) {
		params[i] = i;

		if (pthread_create (&threads[i], NULL, &thread_function, &params[i])) {
			perror ("pthread_create");
			exit (EXIT_FAILURE);
		}
	}
	
	for (i = 0 ; i < number_of_threads ; ++i)
		if (pthread_join (threads[i], NULL)) {
			perror ("pthread_join");
			exit (EXIT_FAILURE);
		}
	
	free (search);
	free (num);
	free (threads);
	free (params);
	
	return EXIT_SUCCESS;
}
