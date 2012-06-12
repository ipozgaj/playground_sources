/* get_info.c - get host IP from host name
 *
 * (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
	int i;
	char **tmp;	
	struct hostent *host_info;
	
	extern int h_errno;
	
	if (argc != 2) {
		fprintf (stderr, "Usage: get_ip hostname\n");
		exit (EXIT_FAILURE);
	}

	host_info = gethostbyname (argv[1]);
	
	if (host_info == NULL) {
		switch (h_errno) {
			case HOST_NOT_FOUND:
				fprintf (stderr, "The specified host is unknown\n");
				break;
					
			case NO_ADDRESS:
				fprintf (stderr, "The requested name is valid but does not have an IP address\n");
				break;
				
			case NO_RECOVERY:
				fprintf (stderr, "A non recoverable name server error occured\n");
				break;
				
			case TRY_AGAIN:
				fprintf (stderr, "A temporary error occured on an authorative name server\n");
				break;
				
			default:
				abort ();
		}
		
		exit (EXIT_FAILURE);
	}
	
	printf ("Host official name: %s\n", host_info->h_name);
	
	for (i=1, tmp = host_info->h_aliases ; *tmp != NULL ; ++tmp, ++i)
		printf ("Alias #%i: %s\n", i, *tmp);
	
	if (host_info->h_addrtype = AF_INET)
		printf ("Address type: IPv4\n");
	else
		printf ("Address type: IPv6\n");
						
	printf ("Address length: %d bytes\n", host_info->h_length);
	
	for (i=1, tmp = host_info->h_addr_list ; *tmp != NULL ; ++tmp, ++i) 
		printf ("Address #%i: %s\n", i, *tmp);
	
	return EXIT_SUCCESS;
}
