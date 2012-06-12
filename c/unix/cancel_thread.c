/* cancel_thread.c - creation of synchronous canceable thread, example of
 * protecting critical section inside of thread
 * 
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdlib.h>
#include <pthread.h>

void *thread_function (void *unused)
{
	int old_cancel_state;
	
	/* Set thread type to synchronously cancelable */
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	
	/* start critical section, disable thread cancelation */
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
	
	/* Critical code goes here */
	
	/* reenable tread cancelation */
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_cancel_state);
	
	/* create cancelation point */
	pthread_testcancel();

	/* do some more work that is not critical*/

	return NULL;
}

int main ()
{
	pthread_t thread;

	pthread_create(&thread, NULL, thread_function, NULL);
  
	return EXIT_SUCCESS;
}
