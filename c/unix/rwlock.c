/* rwlock - example on using ReadWrite locks
 *
 * (C) 2009  Igor Pozgaj <ipozgaj@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int shared_data = 0;

pthread_rwlock_t lock;

void *locker_thread()
{
	printf("Writer Thread: Entering locker thread\n");
	
	sleep(1);

	pthread_rwlock_rdlock(&lock);
	printf("Writer Thread: Placed read lock\n");
	sleep(5);
	pthread_rwlock_unlock(&lock);
	printf("Writer Thread: Released read lock\n");

	pthread_rwlock_wrlock(&lock);
	printf("Writer Thread: Placed write lock\n");
	sleep(5);
	pthread_rwlock_unlock(&lock);
	
	printf("Writer Thread: Released write lock\n");

	return NULL;
}

void *reader_thread()
{
	int i;

	for (i = 0 ; i < 10 ; ++i) {
		pthread_rwlock_rdlock(&lock);
		printf("Reader Thread: Placed read lock\n");
		pthread_rwlock_unlock(&lock);
		sleep(1);
	}

	return NULL;
}

int main()
{
	pthread_t threads[2];

	pthread_rwlock_init(&lock, NULL);

	pthread_create(&threads[0], NULL, locker_thread, NULL);
	pthread_create(&threads[1], NULL, reader_thread, NULL);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	pthread_rwlock_destroy(&lock);

	return EXIT_SUCCESS;
}
