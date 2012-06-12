/* dka.c - deterministicki konacni automat
 * 
 * (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * Neka je zadan jezik L nad skupom dekadskih znamenaka {0, 1, ..., 9}.
 * Niz se promatra kao cjelobrojna vrijednost. Niz je u jeziku L ako je
 * njegova cjelobrojna vrijednost djeljiva s tri, L = {e, 0, 3, 6, ...}
 * Implementirati DKA koji prihvaca zadani jezik L.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STATES       3
#define TRANSITIONS  10

int main ()
{
	int in, q;
	
	/* tablica prijelaza */
	int tt[STATES][TRANSITIONS] = {
		{0, 1, 2, 0, 1, 2, 0, 1, 2, 0},
		{1, 2, 0, 1, 2, 0, 1, 2, 0, 1},
		{2, 0, 1, 2, 0, 1, 2, 0, 1, 2}
	};
	
	/* Prazni niz je po definiciji prihvatljiv */
	q = 0;
	
	printf ("Unesi niz: ");
	
	while ((in = fgetc(stdin)) != EOF && in != '\n') {
		if (!(isdigit (in))) {
			fprintf (stderr, "Zadani niz sadrzi znakova izvan abecede\n");
			exit (EXIT_FAILURE);
		}
		
		/* prijelaz u novo stanje */
		q = tt[q][in - '0'];
	}
	
	if (q == 0) 
		printf ("Niz je unutar jezika\n");
	else
		printf ("Niz nije unutar jezika\n");
	
	return EXIT_SUCCESS;
}
