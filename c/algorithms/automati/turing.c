/* turing.c - gramatika neogranicenih produkcija iz Turingovog stroja
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
	int stanje;
	char ulazni_znak;
	char pomak;
} konfiguracija_t;

int main (int argc, char **argv)
{
	int broj_stanja, *prihvatljiva_stanja = NULL, tmp, i = 0, j, k, l, n = 0;
	int broj_prihvatljivih_stanja;
	char *ulazni_znakovi = NULL, *znakovi_trake = NULL, ch;
	FILE *in, *out;

	konfiguracija_t konf, *shema = NULL;
	
	if (argc != 3) {
		fprintf (stderr, "Koristenje: turing [ulazna_datoteka] [izlazna_datoteka]\n");
		exit (EXIT_FAILURE);
	}
		
	if ((in = fopen (argv[1], "r")) == NULL) {
		perror (argv[1]);
		exit (EXIT_FAILURE);
	}
	
	if ((out = fopen (argv[2], "w")) == NULL) {
		perror (argv[2]);
		exit (EXIT_FAILURE);
	}

	fscanf (in, "%d,{", &broj_stanja);
			
	while (1) {
		fscanf (in, "%c", &ch);

		if (ch == ',') continue;
		
		assert (ulazni_znakovi = realloc (ulazni_znakovi, ++n));
		
		if (ch == '}') {
			ulazni_znakovi[n-1] = '\0';
			break;
		}

		ulazni_znakovi[n-1] = ch;
	}

	n = 0;
	
	while (1) {
		fscanf (in, "%c", &ch);
		
		if (ch == ',' || ch == '{') continue;
		
		assert (znakovi_trake = realloc (znakovi_trake, ++n));

		if (ch == '}') {
			znakovi_trake[n-1] = '\0';
			break;
		}

		znakovi_trake[n-1] = ch;
	}

	fscanf (in, ",{");
	
	n = 0;
	
	while (1) {
		fscanf (in, "%d", &tmp);

		assert (prihvatljiva_stanja = realloc (prihvatljiva_stanja, ++n * sizeof (int)));
		
		prihvatljiva_stanja[n-1] = tmp;
		
		fscanf (in, "%c", &ch);

		if (ch == ',') continue;
		if (ch == '}') break;
	}
	
	broj_prihvatljivih_stanja = n;
	
	fscanf (in, "%c", &ch);
	
	assert (shema = malloc (broj_stanja * strlen (znakovi_trake) * sizeof (konfiguracija_t)));
	
	while(1) {
		if ((fscanf (in, "%c", &ch)) == EOF)
			break;

		if (ch == '-') {
			konf.stanje = -1;
			konf.ulazni_znak = konf.pomak = ' ';
			fscanf (in, "%c", &ch);
			*(shema+i) = konf;
			++i;
			continue;
		} else {
			if (ch == '\n') 
				continue;
			else
				fseek (in, -1, SEEK_CUR);
		}
		
		fscanf (in, "%d,", &konf.stanje);
		fscanf (in, "%c,", &konf.ulazni_znak);
		fscanf (in, "%c/\n", &konf.pomak);
		
		*(shema+i) = konf;
		++i;
	};
	
	fprintf (out, "[A1] --> [q0][A2]\n");
	for (i=0 ; i < strlen (ulazni_znakovi) ; ++i)
		fprintf (out, "[A2] --> [%c,%c][A2]\n", ulazni_znakovi[i], ulazni_znakovi[i]);
	fprintf (out, "\n");
	
	fprintf (out, "[A2] --> [A3]\n");
	fprintf (out, "[A3] --> [$,B][A3]\n");
	fprintf (out, "[A3] --> $\n");
	fprintf (out, "\n");
	
	ulazni_znakovi = strdup (strcat (ulazni_znakovi, "$"));
	
	for (i=0 ; i<broj_stanja * strlen (znakovi_trake) ; ++i) {
		konf = shema[i];
			
		if (konf.stanje == -1) continue;

		if (konf.pomak == 'R') {
			for (j=0 ; j<strlen (ulazni_znakovi) ; ++j) {
					fprintf (out, "[q%d]", i/strlen(znakovi_trake));
					fprintf (out, "[%c,%c] --> ", ulazni_znakovi[j], znakovi_trake[i % strlen(znakovi_trake)]);
					fprintf (out, "[%c,%c]", ulazni_znakovi[j], konf.ulazni_znak);
					fprintf (out, "[q%d]\n", konf.stanje);
				}
		} else {
			for (j=0 ; j<strlen (ulazni_znakovi) ; ++j)
				for (k=0 ; k<strlen (ulazni_znakovi) ; ++k) 
					for (l=0 ; l<strlen (znakovi_trake); ++l) {
						fprintf (out, "[%c,%c]", ulazni_znakovi[k], znakovi_trake[l]);
						fprintf (out, "[q%d]", i/strlen(znakovi_trake));
						fprintf (out, "[%c,%c]", ulazni_znakovi[j], znakovi_trake[i % strlen (znakovi_trake)]);
						fprintf (out, " --> [q%d]", konf.stanje);
						fprintf (out, "[%c,%c]", ulazni_znakovi[k], znakovi_trake[l]);
						fprintf (out, "[%c,%c]\n", ulazni_znakovi[j], konf.ulazni_znak);
					}
		}
	}

	fprintf (out, "\n");
	
	for (i=0 ; i<broj_prihvatljivih_stanja ; ++i) {
		for (j=0 ; j<strlen (ulazni_znakovi) ; ++j)
			for (k=0 ; k<strlen (znakovi_trake) ; ++k) {
				fprintf (out, "[%c,%c]", ulazni_znakovi[j], znakovi_trake[k]);
				fprintf (out, "[q%d] --> ", prihvatljiva_stanja[i]);
				fprintf (out, "[q%d]%c[q%d]\n", prihvatljiva_stanja[i], ulazni_znakovi[j], prihvatljiva_stanja[i]);
			}
	}
	
	for (i=0 ; i<broj_prihvatljivih_stanja ; ++i) {
		for (j=0 ; j<strlen (ulazni_znakovi) ; ++j)
			for (k=0 ; k<strlen (znakovi_trake) ; ++k) {
				fprintf (out, "[q%d]", prihvatljiva_stanja[i]);
				fprintf (out, "[%c,%c] --> ", ulazni_znakovi[j], znakovi_trake[k]);
				fprintf (out, "[q%d]%c[q%d]\n", prihvatljiva_stanja[i], ulazni_znakovi[j], prihvatljiva_stanja[i]);
			}
		fprintf (out, "[q%d] --> $\n", prihvatljiva_stanja[i]);
	}
	
	fclose (in);
	fclose (out);
	free (ulazni_znakovi);
	free (znakovi_trake);
	free (prihvatljiva_stanja);
	free (shema);
	
	return EXIT_SUCCESS;
}
