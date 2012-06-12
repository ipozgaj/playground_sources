/* environtment.c - show environment vriables
 *
 * (C) 2003 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>

extern char **environ;

int main()
{
	char **var;

	for (var = environ ; *var ; var++)
		printf("%s\n", *var);
		
	return 0;
}
