/* shell_sort.c - Shell sort algoritam
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shell_sort (int *a, int n)
{
	int i, j, step, tmp;

	for (step=n/2 ; step>0 ; step /= 2) 
		for (i=step ; i<n ; i++) {
			tmp = a[i];

			for (j=i ; j>=step && a[j-step]>tmp; j-=step)
				a[j] = a[j-step];

			a[j] = tmp;
		}
}

int main ()
{
	int i, n;
	int *polje;
	
	do {
		printf ("Unesi velicinu polja: ");
		scanf ("%d", &n);
	} while (n<=0);

	if ((polje = malloc (n * sizeof (int))) == NULL) {
		fprintf (stderr, "Ne mogu alocirati polje");
		exit (1);
	}
	
	srand ((unsigned) time (NULL));
	
	printf ("Generiram polje...\n");
	for (i=0 ; i<n ; ++i)
		polje[i] = rand () % 1000;

	printf ("Sortitam polje...\n");
	shell_sort (polje, n);
	
	/* ispisi sortirano polje */
	for (i=0 ; i<n ; i++)
		printf ("%d ", polje[i]);

	printf ("\n");
	
	return 0;
}

