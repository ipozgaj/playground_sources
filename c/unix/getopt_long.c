/* getopt_long.h - command line options parsing with getopt_long()
 * 
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

const char *program_name;

void print_usage(FILE *stream, int exit_code)
{
	fprintf(stream, "Usage: %s [options]\n", program_name);
	fprintf(stream, "  -h  --help     Display this usage information\n");
	fprintf(stream, "  -v  --version  Display version\n");

	exit(exit_code);	
}

void print_date()
{
	time_t date;
	
	date = time(&date);
	
	printf("%s\n", ctime(&date));
}

int main(int argc, char **argv)
{
	int next_option;
	
	const char *short_options = "hv";

	const struct option long_options[] = {
		{"help",    0, NULL, 'h'},
		{"version", 0, NULL, 'v'},
		{NULL,      0, NULL,  0}
	};

	program_name = argv[0];

	do {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);

		switch (next_option) {
			/* -h or --help */	
			case 'h':
				print_usage(stdout, 0);
				break;

			/* -v or --version */
			case 'v':
				printf("Version 1.0\n");
				exit(0);
				break;

			/* Nonexistent option */
			case '?': 
				print_usage(stderr, 1);
				break;

			/* Done with parsing */
			case -1:
				break;

			/* Something unexpected occured */
			default:
				abort();
		}
	} while (next_option != -1);
	
	print_date();
	
	return EXIT_SUCCESS;
}

