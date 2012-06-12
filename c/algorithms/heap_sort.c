/* heap_sort.c - sortiranje gomilom
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

/* Podesavanje gomile. Prvi element polja mora imati indeks 1! */
void adjust_heap (int *a, int i, int n)
{
	int j, tmp;

	j = i*2;

	tmp = a[i];

	while (j<=n) {
		if (j<n && (a[j] < a[j+1])) j++;
		if (tmp > a[j]) break;
		a[j/2] = a[j];
		j *= 2;
	}

	a[j/2] = tmp;
}

/* Napravi gomilu */
void make_heap (int *a, int n)
{
	int i;

	for (i=n/2 ; i>0 ; i--)
		adjust_heap (a, i, n);
}

void heap_sort (int *a, int n)
{
	int i;

	make_heap (a, n);

	for (i=n ; i>1 ; i--) {
		swap (&a[1], &a[i]);
		adjust_heap (a, 1, i-1);
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
	heap_sort (polje-1, n);
	
	/* ispisi sortirano polje */
	for (i=0 ; i<n ; i++)
		printf ("%d ", polje[i]);

	printf ("\n");
	
	return 0;
}

