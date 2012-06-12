/* presjek.c - nalazenje presjeka dva skupa. Primjer za razne 
 *             apriorne slozenosti algoritama
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>

/* Vraca 0 ako su arg1=arg2, -1 ako arg1<arg2, 1 ako arg1>arg2 */
int compare (const void *arg1, const void *arg2)
{
	if (*(int*) arg1 == *(int *) arg2) 
		return 0;
	else 
		if (*(int *) arg1 < *(int *) arg2)
			return -1;
		else 
			return 1;
}

/* Brute-force pristup, apriorna slozenost O(n*n) */
int presjek1 (int *a, int *b, size_t n, int **p)
{
	int i, j, k;

	/* broj zajednickih elementata */
	k = 0;
	
	/* Usporedi svaki element iz a sa svakim iz b */
	for (i=0 ; i<n ; i++)
		for (j=0 ; j<n ; j++)
			if (a[i] == b[j]) {
				/* Dinamicki promjeni velicinu spremnika ze presjek */
				*p = realloc (*p, (k + 1) * sizeof (int));
				(*p)[k] = a[i];
				++k;
			}
	
	return k;
}

/* Usporedba prethodno sortiranih polja, slozenost O(n) */
int presjek2 (int *a, int *b, size_t n, int **p)
{
	int i, j, k;

	i = j = k = 0;

	while (i<n && j<n) {
		if (a[i] == b[j]) {
			*p = realloc (*p, (k+1) * sizeof (int));
			(*p)[k] = a[i];
			++i;
			++j;
			++k;
		} else
			if (a[i] < b[j]) {
				++i;
			} else {
				++j;
			}
	}

	return k;
}

int main ()
{
	int polje1[] = {1, 8, 2, 22, 19};
	int polje2[] = {2, 19, 3, 34, 35};
	int *presjek = NULL;
	int i;
	size_t velicina;
	
	/* sortiraj polja */
	/* qsort ima O(n * lg n) */
	qsort (polje1, 5, sizeof (int), compare);
	qsort (polje2, 5, sizeof (int), compare);

	velicina = presjek2 (polje1, polje2, 5, &presjek);

	for (i=0 ; i<velicina ; i++)
		printf ("%d ", presjek[i]);

	puts ("");
	
	return 0;
}
