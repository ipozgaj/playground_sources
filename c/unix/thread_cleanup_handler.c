/* thread_cleanup_handler.c - example of using cleanup handler for thread
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdlib.h>
#include <pthread.h>

void *allocate_buffer(size_t size)
{
	return malloc(size);
}

void deallocate_buffer(void *buffer)
{
	free(buffer);
}

void *thread_function(void *unused)
{
	void *temp_buffer = allocate_buffer(1024);

	/* register a cleanup handler and make sure buffer gets deallocated */
	pthread_cleanup_push(deallocate_buffer, temp_buffer);

	/* some work */

	/* deregister cleanup hadnler, as we pass non zero argument it 
	will invoke the handler itself before deregistering */
	pthread_cleanup_pop(1);
}

int main()
{
	pthread_t thread;

	pthread_create(&thread, NULL, thread_function, NULL);

	return EXIT_SUCCESS;
}
