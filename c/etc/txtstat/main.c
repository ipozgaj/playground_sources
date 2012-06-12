/* txtstat - statistical analysis of text files
 * Copyright (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, 
 * or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */												
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#define VERSION "1.1.2"

void display_usage (FILE *stream, int exit_code)
{
	fprintf (stream, "Usage: txtstat [options] [file(s)]\n"
					"  -h  --help          Display this usage information\n"
					"  -o  --output file   Write output to file\n"
					"  -s  --stream        Use stdin instead of input file\n"
					"  -v  --verbose       Display verbose messages\n"
					"  -V  --version       Display program version\n");
	
	fprintf (stream, "\nReport bugs to <ipozgaj@fly.srk.fer.hr>\n");
	
	exit (exit_code);
}

int main (int argc, char **argv)
{
	FILE *in, *out;
	char *output;
	int next_option;
	int verbose = 0, stream = 0, have_output_file = 0;
	
	/* getopt_long() data */
	const char *short_options = "ho:svV";
	const struct option long_options[] = {
		{"help",    0, NULL, 'h'},
		{"output",  1, NULL, 'o'},
		{"stream",  0, NULL, 's'},
		{"verbose", 0, NULL, 'v'},
		{"version", 0, NULL, 'V'},
		{NULL, 0, NULL, 0}
	};

	if (argc == 1)
		display_usage (stdout, 0);

	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);

		switch (next_option) {
			/* --help or -h */
			case 'h':
				display_usage (stdout, EXIT_SUCCESS);
				break;

			/* --output or -o */
			case 'o':
				have_output_file = 1;
				output = optarg;
				break;
				
			/* --stream or -s */
			case 's':
				/* switch stream input on */
				stream = 1;
				break;

			/* --verbose or -v */
			case 'v':
				/* switch verbose messages on */
				verbose = 1;
				break;

			/* --version or -u */
			case 'V':
				printf ("txtstat version %s\n", VERSION);
				exit (EXIT_SUCCESS);
				break;

			/* Invalid option */
			case '?':
				display_usage (stderr, EXIT_FAILURE);
				break;

			/* Parsing finished? */
			case -1:
				break;
				
			/* An error occured */
			default:
				abort ();
		}
	} while (next_option != -1);
	
	/* No stream and no files specified */
	if (!stream && !argv[optind])
		display_usage (stderr, EXIT_FAILURE);
	
	/* Open output stream if neccessary */
	if (have_output_file) {
		if ((out = fopen (output, "w")) == NULL) {
			fprintf (stderr, "Could not open file for writting: %s\n", output);
			exit (EXIT_FAILURE);
		}
	} else out = stdout;

	if (stream) {
		if (verbose) fprintf (stderr, "Verbose option has no effect with -s option\n");
		analyze (stdin);
	}
	else {
		while (optind < argc) {
			if (verbose) fprintf (stderr, "Processing file: %s\n", argv[optind]);
	
			if ((in = fopen (argv[optind], "r")) == NULL) {
				fprintf (stderr, "Could not open file for reading: %s\n", argv[optind]);
				exit (EXIT_FAILURE);
			}
			
			analyze (in);
			++optind;
		}
	}
	
	display_graph (out);
	display_stats (out);
	
	return EXIT_SUCCESS;
}
