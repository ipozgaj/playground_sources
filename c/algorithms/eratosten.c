/* eratosten.c - trazenje prim brojeva pomocu eratostenovog sita
 *
 * (C) 2003  Igor Pozgaj
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main ()
{
	int i, j, n;
	int *sieve;
	
	/* Odredi velicinu sita */
	printf ("Unesi velicinu sita: ");
	scanf ("%d", &n);

	if ((sieve = malloc (n * sizeof (int))) == NULL) {
		fprintf (stderr, "Ne mogu alocirati polje\n");
		exit (1);
	}

	/* Inicijalno su svi brojevi prim */
	for (i=1 ; i<n ; ++i)
		sieve[i] = 1;
	
	/* Za svaki prim broj ukloni sve njegove visekratnike */
	for (i=2 ; i<=sqrt(n) ; ++i) {
		if (sieve[i] == 1)
			for (j=2 ; j<=n/i ; ++j)
				sieve[i*j] = 0;
	}

	/* Ispisi prim brojeve */
	for (i=1 ; i<n ; ++i)
		if (sieve[i] == 1)
			printf ("%d ", i);
	
	printf ("\n");
	
	free (sieve);
	
	return 0;
}
