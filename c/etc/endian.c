/* endian.c - determine endianess of system
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
	union {
		short num;
		char bytes[sizeof (short)];
	} un;

	un.num = 0x0102;

	if (sizeof (short) == 2) {
		if (un.bytes[0] == 1 && un.bytes[1] == 2)
			printf ("Big endian\n");
		else if (un.bytes[0] == 2 && un.bytes[1] == 1)
			printf ("Little endian\n");
		else 
			printf ("Confused... Not little nor big endian?!?\n");
	} else {
		printf ("Size of short int is not 2 bytes\n");
		return EXIT_FAILURE;
	}
		
	return EXIT_SUCCESS;
}
