/* tsp.c - problema trgovackog putnika:
 * 
 * (C) 2003 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * Zadan je skup gradova G i cijene putovanja c(i,j) za putovanje iz
 * grada i u grad j. Potrebo je, krenuvsi iz jednog grada. obici sve
 * gradove tocno jednom tako da je ukupni trosak puta najmanji
 *
 *    c(i,j) = c(j,i)
 *    TSP (G(i), G) = min (c(i,j) + TSP (G(j), G \ G(j))
 *    TSP (G(i), {G(j)}) = c(i,j)
 *
 * Slozenost rijesenja (algoritma) je faktorijelna, O((n-1)!) 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Slozenost je n! */
#define MAX_GRAD 15

/* c - matrica udaljenosti izmedju gradova (i, j) */
int c[MAX_GRAD][MAX_GRAD];

/* Operacija skup \ element */
int minus (int *skup, int n, int element)
{
	int i, j;

	for (i=0 ; i<n ; i++)
		if (skup[i] == element)
			break;

	for (j=i ; j<n-1 ; j++)
		skup[j] = skup[j+1];
}

/* Rjesnje problema, rekurzivno */
int tsp (int polaziste, int *gradovi, int n)
{
	int *lgradovi, min_tsp, pom_tsp, i;

	lgradovi = (int *) malloc (n * sizeof (int));
	memcpy (lgradovi, gradovi, n * sizeof (int));
	minus (lgradovi, n, polaziste);
	--n;

	if (n == 1) {
		min_tsp = c[polaziste][lgradovi[0]];
	} else {
		min_tsp = c[polaziste][lgradovi[0]] + tsp (lgradovi[0], lgradovi, n);

		for (i=0 ; i<n ; i++) {
			pom_tsp = c[polaziste][lgradovi[i]] 
				+ tsp (lgradovi[i], lgradovi, n);

			if (pom_tsp < min_tsp) min_tsp = pom_tsp; 
		}
	}
	
	free (lgradovi);
	return min_tsp;
}

int main ()
{
	int gradovi[MAX_GRAD];
	int i, j, n, min_tsp;

	srand (time (NULL));
	
	do {
		printf ("Unesi broj gradova: ");
		scanf ("%d", &n);
	} while (n<2 || n>MAX_GRAD);
	
	/* generiraj matricu */
	for (i=0 ; i<n ; i++) {
		gradovi[i] = i;
		c[i][i] = 0;

		for (j=i+1 ; j<n ; j++) {
			c[i][j] = 1 + rand() % 100;
			c[j][i] = c[i][j];
			printf ("c[%d][%d] = %d\n", i, j, c[i][j]);
		}
	}

	min_tsp = tsp (0, gradovi, n);
	printf ("Najmanji trosak: %d\n", min_tsp);
	
	return 0;
}

