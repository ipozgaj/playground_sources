/* simulacija stranicenja algoritmom LFU (Least frequently used)
 *
 * Labos iz Operacijskih Sustava
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGES_NUM	8

#define PAGE_NEW	1
#define PAGE_HIT	2

int display_usage (FILE *stream, int exit_code)
{
	fprintf (stream, "Usage: lfu frames_count requests_num\n");
	exit (exit_code);
}

int main (int argc, char **argv)
{
	int i, j, min, done, action, frames_count, requests_num;
	int *requests, *page_frames;
	int pages[PAGES_NUM+1] = {0};
		
	if (argc != 3)
		display_usage (stdout, EXIT_SUCCESS);
		
	if ((sscanf (argv[1], "%d", &frames_count)) != 1)
		display_usage (stderr, EXIT_FAILURE);
	
	if ((sscanf (argv[2], "%d", &requests_num)) != 1)
		display_usage (stderr, EXIT_FAILURE);
	
	srand ((unsigned) time (NULL));
	
	requests = malloc (requests_num * sizeof (int));
	page_frames = calloc (frames_count, sizeof (int));
	
	printf ("Zahtjevi: ");
					
	for (i=0 ; i<requests_num ; ++i) {
		requests[i] = 1 + rand() % 8;
		printf ("%d", requests[i]);
		if (i != requests_num - 1) printf (", ");
	}

	printf ("\n\n#N\t");

	for (i=0 ; i<frames_count ; ++i)
		printf ("%d\t", i + 1);

	printf ("\n");
	
	for (i=0 ; i<frames_count ; ++i)
		printf ("---------");

	printf ("\n");

	for (i=0 ; i<requests_num ; ++i) {
		printf ("%d", requests[i]);

		done = action = 0;

		for (j=0 ; j<frames_count ; ++j)
			if (requests[i] == page_frames[j]) {
				action = PAGE_HIT;
				++pages[requests[i]];
				done = 1;
				break;
			}

		if (!done) for (j=0 ; j<frames_count ; ++j)
			if (!page_frames[j]) {
				action = PAGE_NEW;
				++pages[requests[i]];
				page_frames[j] = requests[i];
				done = 1;
				break;
			}
		
		if (!done) {
			action = PAGE_NEW;
			for (min = 0, j=1 ; j<frames_count ; ++j)
				if (pages[page_frames[j]] < pages[page_frames[min]])
					min = j;

			pages[page_frames[min]] = 0;
			page_frames[min] = requests[i];
			++pages[requests[i]];
		}
		
		for (j=0 ; j<frames_count ; ++j)
			if (!page_frames[j])
				printf ("\t-");
			else
				if (page_frames[j] == requests[i])
					if (action == PAGE_NEW)
						printf ("\t\b[%d]", page_frames[j]);
					else
						printf ("\t\b(%d)", page_frames[j]);
				else
					printf ("\t%d", page_frames[j]);

		printf ("\n");
	}
	
	return EXIT_SUCCESS;
}
