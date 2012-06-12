/* simulator DKA za 1. labos iz afjjp1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXLEN 1000

int main ()
{
	int i, j, m, n, *tablica, pocetno, stanje;
	FILE *in;
	char *buffer, *s;

	if ((in = fopen ("tablica.dat", "r")) == NULL) {
		perror ("tablica.dat");
		exit (EXIT_FAILURE);
	}
	
	fscanf (in, "%d\n", &n);
	fscanf (in, "%d\n", &m);
	
	assert (s = (char *) malloc (m + 1));
	fgets (s, m + 1, in);
	
	pocetno = stanje = n / 2 - 1;
	
	assert (tablica = (int *) malloc (sizeof (int) * (m+1) * n));
	
	for (i=0 ; i<n ; ++i)
		for (j=0 ; j<m+1 ; ++j)
			fscanf (in, "%d ", &tablica[i * (m+1) + j]);
	
	fclose (in);
	
	printf ("Unesi niz znakova: ");

	assert (buffer = (char *) malloc (MAXLEN));
	fgets (buffer, MAXLEN, stdin);

	
	printf ("\n%d", stanje);
	
	for (j=0 ; j<strlen(buffer)-1 ; ++j) {
		if (strchr (s, buffer[j]))
			for (i=0 ; i<m+1 ; ++i)
				if (s[i] == buffer[j]) {
					stanje = tablica[(m + 1) * stanje + i];
					break;
				}

	printf ("--(%c)-->%d", buffer[j], stanje);
	}
	
	if (stanje == pocetno) 
		printf ("\nNiz se prihvaca\n");
	else
		printf ("\nNiz se ne prihvaca\n");
	
	free (s);
	free (tablica);
	free (buffer);

	return EXIT_SUCCESS;
}
