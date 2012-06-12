/* ntest.c - simple port scaner/flooder and network tester
 * (C) 2004-2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_SIZE 0x200
#define N_FOREVER 0x7FFFFFFF

char *program_name = "ntest";
char *program_version = "0.1.2";

/* Display usage and exit with given exit code */
void display_usage (FILE *stream, int exit_code)
{
	fprintf (stream, "Usage: %s [option(s)] host\n", program_name);
	fprintf (stream, "  -p  --port      Connect to specified port\n");
	fprintf (stream, "  -n  --tries     Number of tries (default is 1)\n");
	fprintf (stream, "  -f  --forever   Unlimited number od tries\n");
	fprintf (stream, "  -m  --message   Text of message to be sent\n");
	fprintf (stream, "  -c  --count     Numbers of message write\n");
	fprintf (stream, "  -F  --flood     Write messages forever\n");
	fprintf (stream, "  -d  --delay     Seconds to wait betwen connect atemps\n");
	fprintf (stream, "  -w  --wait      Seconds to wait upon connect\n");
	fprintf (stream, "  -s  --sleep     Seconds to wait betwen write atemps\n");
	fprintf (stream, "  -v  --verbose   Display verbose messages\n");
	fprintf (stream, "  -h  --help      Display this usage information\n");
	fprintf (stream, "  -V  --version   Display program version\n");

	exit (exit_code);
}

/* Display program version and credits, then exit */
void display_version ()
{
	printf ("%s version %s\n", program_name, program_version);
	printf ("(C) 2004-2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>\n");
	
	exit (EXIT_SUCCESS);
}

void get_host_response (int socket_fd)
{
	char buffer[BUFFER_SIZE];
	ssize_t chars_read_num;

	while (1) {
		chars_read_num = read (socket_fd, buffer, BUFFER_SIZE);

		if (chars_read_num == 0)
			return;

		fwrite (buffer, sizeof (char), chars_read_num, stdout);
	}
}

void send_msg_to_host (int socket_fd, char *msg)
{
	write (socket_fd, msg, strlen (msg));
}

int main (int argc, char **argv)
{
	char *host_name;
	int socket_fd, port_num;
	struct sockaddr_in name;
	struct hostent *hostinfo;
	
	int i, count = 1, tries = 1, verbose = 0;
	int write_delay = 0, atemps_delay = 0, wait_delay = 0;
	char *message;
	
	int next_option;
	char *short_options="p:n:fm:c:Fd:w:s:vhV";
	struct option long_options[] = {
		{"port",    1, NULL, 'p'},
		{"tries",   1, NULL, 'n'},
		{"forever", 0, NULL, 'f'},
		{"message", 1, NULL, 'm'},
		{"count",   1, NULL, 'c'},
		{"flood",   0, NULL, 'F'},
		{"delay",   1, NULL, 'd'},
		{"wait",    1, NULL, 'w'},
		{"sleep",   1, NULL, 's'},
		{"verbose", 0, NULL, 'v'},
		{"help",    0, NULL, 'h'},
		{"version", 0, NULL, 'V'},
		{NULL,      0, NULL,  0}
	};
	
	/* Do command line options parsing */
	if (argc == 1) display_usage (stderr, 1);
	
	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);

		switch (next_option) {
			/* -p or --port */
			case 'p':
				if ((sscanf (optarg, "%d", &port_num)) == 0) {
					fprintf (stderr, "Invalid port number\n");
					exit (EXIT_FAILURE);
				}
				break;
			
			/* -n or --tries */
			case 'n':
				if ((sscanf (optarg, "%d", &tries)) == 0) {
					fprintf (stderr, "Invalid number of tries\n");
					exit (EXIT_FAILURE);
				}
				break;
				
			/* -f or --forever */
			case 'f':
				tries = N_FOREVER;
				break;

			/* -c or --count */
			case 'c':
				if ((sscanf (optarg, "%d", &count)) == 0) {
					fprintf (stderr, "Invalid number of write tries\n");
					exit (EXIT_FAILURE);
				}
				break;

			/* -F or --flood */
			case 'F':
				count = N_FOREVER;
				break;
				
			/* -m or --message */
			case 'm':
				message = (char *) malloc (sizeof (char) * (strlen (optarg) + 2));
				strncpy (message, optarg, sizeof (char) * (strlen (optarg) + 2));
				strcat (message, "\n");
				break;

			/* -w or --wait */
			case 'w':
				if ((sscanf (optarg, "%d", &wait_delay)) == 0) {
					fprintf (stderr, "Invalid number of seconds\n");
					exit (EXIT_FAILURE);
				}
				break;
			
			/* -s or --sleep */
			case 's':
				if ((sscanf (optarg, "%d", &write_delay)) == 0) {
					fprintf (stderr, "Invalid number of seconds\n");
					exit (EXIT_FAILURE);
				}
				break;
			
			/* -d or --delay */
			case 'd':
				if ((sscanf (optarg, "%d", &atemps_delay)) == 0) {
					fprintf (stderr, "Invalid number of seconds\n");
					exit (EXIT_FAILURE);
				}
				break;
		
			/* -h or --help */
			case 'h':
				display_usage (stdout, 0);
				break;

			/* -v or --verbose */
			case 'v':
				verbose = 1;
				break;
				
			/* -V or --version */
			case 'V':
				display_version ();
				break;

			/* Invalid option */
			case '?':
				display_usage (stderr, 1);
				break;

			/* Command line parameters parsing over */
			case -1:
				break;
				
			/* Unexpected condition */
			default:
				abort ();
		}
	} while (next_option != -1);
	
	if (argv[optind]) {
		host_name = (char *) malloc (sizeof (char) * (strlen (argv[optind]) + 1));
		strncpy (host_name, argv[optind], sizeof (char) * (strlen (argv[optind]) + 1));
	} else {
		fprintf (stderr, "No hostname found.\n");
		exit (EXIT_FAILURE);
	}
	
	if (!port_num || port_num<=0 || port_num >=65535) {
		fprintf (stderr, "No port or invalid port specified.\n");
		exit (EXIT_FAILURE);
	}
	
	if (verbose) printf ("Connecting to %s port %d...\n", host_name, port_num);
	
	socket_fd = socket (PF_INET, SOCK_STREAM, 0);

	name.sin_family = AF_INET;
	
	hostinfo = gethostbyname (host_name);

	if (hostinfo == NULL) {
		fprintf (stderr, "Could not connect to remote host.\n");
		exit (EXIT_FAILURE);
	}
	else
		name.sin_addr = *((struct in_addr*) hostinfo->h_addr);

	name.sin_port = htons (port_num);

	for (i=0 ; i<tries ; i++) {
		if (verbose && tries!=1) fprintf (stderr, "Try %d of %d: ", i+1, tries);
		if (connect (socket_fd, (struct sockaddr *) &name, sizeof (struct sockaddr_in)) == -1) {
			perror (host_name);
			if (i == tries - 1) exit (EXIT_FAILURE);
		} else break;
		if (atemps_delay) sleep (atemps_delay);
	} 

	printf ("Connected.\n");

	if (verbose && wait_delay!=0) 
		printf ("Sleeping %d seconds...\n", wait_delay);

	if (wait_delay) sleep (wait_delay);	
	
	for (i=0 ; i<count ; i++) {
		if (verbose && count>1) fprintf (stderr, "Write %d of %d: ", i+1, count);
		if (verbose) fprintf (stderr, "Sending message: %s", message);
		send_msg_to_host (socket_fd, message);
		if (write_delay) sleep (write_delay);

	}
	
	if (verbose) fprintf (stderr, "Waiting for server respose\n");
	get_host_response (socket_fd);

	free (host_name);
	
	return 0;
}
