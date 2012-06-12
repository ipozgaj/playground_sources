/* mutex.c - thread synchronization with mutex
 * 
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <malloc.h>
#include <pthread.h>

struct node {
	int key;
	struct node *next;
} *head;

/* alternative is to call pthread_mutex_init() */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *unused)
{
	struct node *next_node;
	
	while (1) {
		/* lock the mutex */
		pthread_mutex_lock(&mutex);
    
		if (head == NULL) {
			next_node = NULL;
		} else {
			next_node = head;
			head = head->next;
		}

		/* Unlock the mutex */
		pthread_mutex_unlock(&mutex);
    
		if (next_node == NULL)
			break;

		free (next_node);
	}
  
	return NULL;
}

int main()
{
	pthread_t thread;

	/* insert queue initialization here */

	pthread_create(&thread, NULL, thread_function, NULL);


	return 0;
}
