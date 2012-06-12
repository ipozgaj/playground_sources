/* mkstemp.c - create an unique temporary file by using mkstemp system call
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd;
	char filename_template[] = "tempXXXXXX";

	fd = mkstemp(filename_template);
	printf("Temporary filename: %s\n", filename_template);
	
	return 0;		
}
