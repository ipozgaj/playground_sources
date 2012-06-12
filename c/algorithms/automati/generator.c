/* generator DKA za 1. labos iz afjjp */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int kodiraj_stanje (int *parovi, int broj_parova, int m)
{
	int i, kod;

	for (kod=0, i=0 ; i<broj_parova ; ++i) {
		if (fabs(parovi[i])>m) return pow (2 * m + 1, broj_parova);
		kod += (parovi[i] + m) * pow (2 * m + 1, i);
	}
	return kod;
}

void dekodiraj_stanje (int kod, int *parovi, int n, int m)
{
	int tmp_kod, i;

	tmp_kod = kod;
	
	for (i=0 ; i<n ; ++i) {
		parovi[i] = (tmp_kod % (2 * m + 1)) - m;
		tmp_kod /= 2 * m + 1;
	}
}

int main ()
{
	FILE *in, *out;
	int m, n, i, stanje, *parovi, broj_stanja;
	char ch, *s = NULL;
	
	if ((in = fopen ("ulaz.dat", "r")) == NULL) {
		perror ("ulaz.dat");
		exit (EXIT_FAILURE);
	}
	
	fscanf (in, "%d\n", &m);

	n = 0;
	
	while (fscanf (in, " %c-", &ch) != EOF) {
		s = (char *) realloc (s, ++n);
		*(s + n - 1) = ch;
	}
	
	*(s + n) = '\0';

	fclose (in);
	
	n /= 2;
	
	parovi = (int *) malloc (n);
	broj_stanja = pow ((2 * m + 1), n);

	printf ("Broj generiranih stanja: %d\n", broj_stanja + 1);

	if ((out = fopen ("tablica.dat", "w")) == NULL) {
		perror ("izlaz.dat");
		exit (EXIT_FAILURE);
	}
	
	fprintf (out, "%d\n", broj_stanja + 1);
	fprintf (out, "%d\n", n * 2);
	fprintf (out, "%s\n", s);

	for (stanje=0 ; stanje<broj_stanja ; ++stanje) {
		for (i=0 ; i<strlen(s) ; ++i) {
			dekodiraj_stanje (stanje, parovi, n, m);
			
			if (i%2) 
				--parovi[i/2];
			else
				++parovi[i/2];
			
			fprintf (out, "%d ", kodiraj_stanje (parovi, n, m));
		}
							
		fprintf (out, "%d\n", stanje);
	}
		
	for (i=0 ; i<strlen(s) + 1 ; ++i)
		fprintf (out, "%d ", broj_stanja);
	
	fprintf (out, "\n");

	fclose (out);
	
	return EXIT_SUCCESS;
}
