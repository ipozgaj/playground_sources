/* bin_search.c - binarno pretrazivanje
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* vraca indeks u polju gdje se nalazi element (-1 ako ga nema) */
/* polje MORA biti sortirano */
int bin_search (int x, int *polje, int n)
{
	int pos, lpos, rpos;

	pos = n / 2;
	lpos = 0;
	rpos = n;

	while (lpos != rpos - 1) {
		if (x > polje[pos]) {
			lpos = pos;
			pos = (pos + rpos) / 2;
		} else if (x < polje[pos]) {
			rpos = pos;
			pos = (pos + lpos) / 2;
		} else 
			return pos;
	}

	return -1;
}

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

	/* polje MORA biti sortirano */
	printf ("Sortitam polje...\n");
	shell_sort (polje, n);
	
	for (i=0 ; i<n ; ++i)
		printf ("%d ", polje[i]);

	printf ("\n");		
	
	printf ("Unesi broj koji treba pronaci: ");
	scanf ("%d", &i);
	
	i = bin_search (i, polje, n);

	if (i == -1) 
		printf ("Nema takvog elementa");
	else
		printf ("Element je na mjestu %d", i+1);
	
	printf ("\n");
	
	return 0;
}

