/* selection_sort.c - sortiranje odabiranjem
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* zamjeni varijable a i b */
void swap (int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void selection_sort (int *a, int n)
{
	int i, j, min;

	for (i=0 ; i<n-1 ; i++) {
		min = i;

		for (j=i+1 ; j<n ; j++)
			if (a[j] < a[min]) min = j;

		swap (&a[i], &a[min]);
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
	selection_sort (polje, n);
	
	/* ispisi sortirano polje */
	for (i=0 ; i<n ; i++)
		printf ("%d ", polje[i]);

	printf ("\n");
	
	return 0;
}

