/* check_jmbg - provjera ispravnosti JMBG-a
 *
 * (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define JMBG_SIZE 13

int main ()
{
	int i, sum;
	char buffer[JMBG_SIZE+1];
	int jmbg[JMBG_SIZE];
	
	printf ("Unesi JMBG: ");
	fgets (buffer, JMBG_SIZE+1, stdin);

	for (i=0 ; i<JMBG_SIZE ; ++i) {
		if (!isdigit(buffer[i])) {
			fprintf (stderr, "Neispravno unesen JMBG\n");
			exit (EXIT_FAILURE);
		}
		
		jmbg[i] = buffer[i] - '0';
	}
	
	for (sum=0, i=2 ; i<8 ; ++i) 
		sum += i * (jmbg[7-i] + jmbg[13-i]);

	sum = 11 - (sum % 11);

	if (sum == jmbg[JMBG_SIZE-1])
		printf ("JMBG je ispravan\n");
	else
		printf ("JMBG nije ispravan\n");
	
	return EXIT_SUCCESS;
}

