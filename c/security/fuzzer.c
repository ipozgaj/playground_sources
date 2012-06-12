/* fuzzer.c
 * (C) 2009 Igor Pozgaj <ipozgaj@gmail.com>
 *
 * gcc -c -fPIC fuzzer.c
 * ld -shared -o fuzzer.so fuzzer.o
 */

#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE	0x100

char *p;

char *getenv(const char *name)
{
	if (!strncmp(name, "DISPLAY", 7))
		return ":0";

	return p;
}

void _init()
{
	p = malloc(BUFFER_SIZE);
	memset(p, 'A', BUFFER_SIZE);
}
