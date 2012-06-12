/* popen.c - redirection with popen
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <unistd.h>

int main ()
{
	FILE *stream;

	stream = popen("sort", "w");

	fprintf(stream, "This\n");
	fprintf(stream, "should\n");
	fprintf(stream, "be\n");
	fprintf(stream, "sorted\n");

	return pclose(stream);
}
