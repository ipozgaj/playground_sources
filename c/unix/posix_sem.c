/* posix_sem.c - thread synchronization with POSIX semaphores
 * 
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>

struct node {
	int key;
	struct node *next;
} *head;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t node_count;

/* process jobs until list becomes emptry */
void *thread_function(void *unused)
{
	struct node *next_node;
	
	while (1) {
		/* wait for new job */
		sem_wait(&node_count);
    
		/* lock mutex */
		pthread_mutex_lock(&mutex);
    
		/* there is no need to check if queue is empty
		   as semaphore guaranties that */
		next_node = head;
		head = head->next;

		/* release mutex */
		pthread_mutex_unlock(&mutex);
    
		/* Ubaciti neki kod za obradu cvora next_node */

		printf("%d\n", next_node->key);

		free(next_node);
	}
  
	return NULL;
}

void initialize_list ()
{
	head = NULL;
  
	sem_init(&node_count, 0, 0);
}

void add_node(int key)
{
	struct node *t;

	/* alocate new node */
	t = (struct node *) malloc(sizeof (struct node));

	t->key = key;

	pthread_mutex_lock(&mutex);
  
	t->next = head;
	head = t;

	/* signal that there is a new node available) */
	sem_post(&node_count);

	pthread_mutex_unlock(&mutex);
}

int main()
{
	int i;

	pthread_t thread;

	pthread_create(&thread, NULL, thread_function, NULL);

	for (i = 0 ; i < 10 ; i++)
		add_node(i);

	return 0;
}
