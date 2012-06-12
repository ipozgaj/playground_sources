/* thread_specific.c - storing data to thread specific data area
 * 
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <pthread.h>

static pthread_key_t thread_log_key;

void write_log(const char *msg)
{
	FILE *thread_log;

	thread_log = (FILE *) pthread_getspecific(thread_log_key);
  
	fprintf(thread_log, "%s\n", msg);
}

void close_log(void *thread_log)
{
	fclose((FILE *) thread_log);
}

void *thread_function(void *unused)
{
	FILE *thread_log;
	char filename[20];

	sprintf(filename, "thread%d.log", (int) pthread_self());
	thread_log = fopen(filename, "w");
  
	pthread_setspecific(thread_log_key, thread_log);
  
	write_log("Thread started");
  
	return NULL;
}

int main()
{
	int i;
	pthread_t threads[5];

	pthread_key_create(&thread_log_key, close_log);

	for (i = 0 ; i < 5 ; i++) {
		pthread_create(&(threads[i]), NULL, thread_function, NULL);
		pthread_join(threads[i], NULL);
	}

	return 0;
}


