/* condvar.c - thread synchronization with conditional variables
 *
 * (C 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;
int producer_done = 0;

void *consumer_thread()
{
	pthread_mutex_lock(&mutex);
	while(!producer_done)
		pthread_cond_wait(&condvar, &mutex);
	printf("Consumer lock\n");
	pthread_mutex_unlock(&mutex);
}

void *producer_thread()
{
	sleep(10);
	pthread_mutex_lock(&mutex);
	printf("Producer lock\n");
	producer_done = 1;
	pthread_cond_signal(&condvar);
	pthread_mutex_unlock(&mutex);
}

int main()
{
	pthread_t consumer, producer;

	pthread_create(&consumer, NULL, &consumer_thread, NULL);
	pthread_create(&producer, NULL, &producer_thread, NULL);

	pthread_join(consumer, NULL);
	pthread_join(producer, NULL);

	return EXIT_SUCCESS;
}
