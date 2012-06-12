/* merge_sort.c - sortiranje uparivanjem
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge (int *a, int *tmp, int lpos, int rpos, int rend)
{
	int i, lend, n, tmppos;

	lend = rpos - 1;
	tmppos = lpos;
	n = rend - lpos + 1;

	while (lpos <= lend && rpos <= rend) {
		if (a[lpos] < a[rpos])
			tmp[tmppos++] = a[lpos++];
		else
			tmp[tmppos++] = a[rpos++];
	}

	while (lpos <= lend)
		tmp[tmppos++] = a[lpos++];

	while (rpos <= rend)
		tmp[tmppos++] = a[rpos++];

	for (i=0 ; i<n ; i++, rend--)
		a[rend] = tmp[rend];
}

/* Rekurzivno sortiranje podpolja */
void msort (int *a, int *tmp, int lijevo, int desno)
{
	int sredina;

	if (lijevo < desno) {
		sredina = (lijevo + desno) / 2;
		msort (a, tmp, lijevo, sredina);
		msort (a, tmp, sredina+1, desno);
		merge (a, tmp, lijevo, sredina +1, desno);
	}
}

void merge_sort (int *a, int n)
{
	int *tmp;

	tmp = (int *) malloc (n * sizeof (int));

	if (tmp) {
		msort (a, tmp, 0, n-1);
		free (tmp);
	} else {
		printf ("Ne mogu stvoriti pomocno polje za Merge sort\n");
		exit (EXIT_FAILURE);
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
	merge_sort (polje, n);
	
	/* ispisi sortirano polje */
	for (i=0 ; i<n ; i++)
		printf ("%d ", polje[i]);

	printf ("\n");
	
	return 0;
}

