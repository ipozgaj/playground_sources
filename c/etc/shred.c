/* shred.c 
 *
 * Program prepisuje sadrzaj datoteke s nasumicno odabranim znakovima cime 
 * onemogucava njeno naknadno rekonstruiranje s diska.
 * 
 * Autor: Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * Zadnja izmjena: 25.6.2003.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>

const char *program_name;

void print_usage (FILE *stream, int exit_code) 
{
	fprintf (stream, "Usage: %s options [inputfile...]\n", program_name);
	fprintf (stream, "  -h  --help     Display this usage information\n"
					 "  -v  --verbose  Print verbose messages\n"
					 "  -n  --passes   Number of overwrites [default=25]\n");
	
	exit (exit_code);
}

long get_file_size (FILE *stream) 
{	
	int current_position;
	long size;
		
	current_position = ftell (stream);
	fseek (stream, 0L, SEEK_END);
	size = ftell (stream);
	
	/* return original position */
	fseek (stream, current_position, SEEK_SET);
					
	return size;
}

int main (int argc, char **argv) 
{		
	int next_option;
	int verbose = 0;
	int i, j, k;

	FILE *target;
	
	/* Default number of overwrites */
	int passes = 30;
	
	const char *short_options = "hvn:";
	const struct option long_options[] = {
		{"help",    0, NULL, 'h'},
		{"verbose", 0, NULL, 'v'},
		{"passes",  1, NULL, 'n'},
		{NULL,      0, NULL,  0 }
	};
	
	program_name = argv[0];

	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);

		switch (next_option) {
			/* -h or --help */
			case 'h':
				print_usage (stdout, 0);
				break;
				
			/* -v or --verbose */
			case 'v':
				verbose = 1;
				break;

			/* -n or --passes */
			case 'n':
				passes = *optarg;
				break;
			
			/* Invalid option */
			case '?':
				print_usage (stderr, 1);
				break;
				
			/* No more options */
			case -1:
				break;
				
			/* Unexpected */
			default:
				abort ();
		}
	} while (next_option != -1);
	
	srand ((unsigned) time (NULL));
	
	for (i = optind ; i < argc ; i++) {
		if ((target = fopen (argv[i], "r+")) != NULL) {
			for (j = 0 ; j < passes ; ++j) {
				if (verbose)
					printf ("%s:\n", argv[i]);
				
				if (verbose) {
					printf ("Pass %d of %d\n", j+1, passes);
					fflush (stdout);
				};
				
				rewind (target);
				
				for (k = 0 ; k < get_file_size (target) ; k++)
					fputc (rand () % 255, target);
			}
			
			fclose (target);
		}
		else
			fprintf (stderr, "%s: no such file\n", argv[i]);
	}
	
	return 0;
}

