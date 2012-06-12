/* quick_sort.c - brzo sortiranje
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Zamijeni sadrzaj varijabli a i b */
void swap (int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/* Sort umetanjem */
void insertion_sort (int *a, int n)
{
	int i, j, tmp;

	for (i=1 ; i<n ; i++) {
		tmp = a[i];

		for (j=i; j>=1 && a[j-1]>tmp ; j--)
			a[j] = a[j-1];

		a[j] = tmp;
	}
}


/* Median funkcija za Quicksort */
int median (int *a, int left, int right)
{
	int middle;

	middle = (left + right) / 2;

	if (a[left] > a[middle]) swap (&a[left], &a[middle]);
	if (a[left] > a[right]) swap (&a[left], &a[right]);
	if (a[middle] > a[right]) swap (&a[middle], &a[right]);

	swap (&a[middle], &a[right-1]);

	return a[right-1];
}

/* Quick sort */
#define CUTOFF (3)

void quick_sort (int *a, int left, int right)
{
	int i, j, m;

	if (left + CUTOFF <= right) {
		m = median (a, left, right);
		i = left;
		j = right - 1;

		while (1) {
			while (a[++i] < m);
			while (a[--j] > m);
			if (i<j) 
				swap (&a[i], &a[j]);
			else
				break;
		}

		swap (&a[i], &a[right-1]);
		quick_sort (a, left, i-1);
		quick_sort (a, i+1, right);
	} else {
		insertion_sort (a + left, right - left + 1);
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
	quick_sort (polje, 0, n);
	
	/* ispisi sortirano polje */
	for (i=0 ; i<n ; i++)
		printf ("%d ", polje[i]);

	printf ("\n");
	
	return 0;
}

