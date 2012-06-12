/* stablo_poljem.c - implementacija binarnog stabla pomocu polja. 
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * Podaci za cvorove se unose iz datoteke "ulaz.dat", zatim se formira 
 * stablo, te ispisu sortirani podaci koristeci inorder prolazak kroz stablo. 
 * Podaci su cjelobrojni brojevi odvojeni razmakom. Prvi podatak u datoteci 
 * je broj cvorova stabla (broj ostalih podataka u datoteci).
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void dodaj_cvor (int key, int root, int *stablo)
{
	if (stablo[root] == 0) {
		stablo[root] = key;
	} else {
		if (key < stablo[root]) {
			dodaj_cvor (key, root*2, stablo);
		} else {
			dodaj_cvor (key, root*2+1, stablo);
		}
	}
}

void inorder (int root, int *stablo)
{
	if (stablo[root*2])
		inorder (root*2, stablo);
	
	if (stablo[root]) printf ("%d ", stablo[root]);
		
	if (stablo[root*2+1])
		inorder (root*2+1, stablo);
}

int main ()
{
	FILE *in;
	int x, n;
	int *stablo;

	if ((in = fopen ("ulaz.dat", "r")) == NULL) {
		fprintf (stderr, "Ulazna datoteka ne postoji\n");
		exit (EXIT_FAILURE);
	}

	fscanf (in, "%d", &n);

	/* korisit calloc umjesto malloc zato da se svi elementi
	 * polja inicijalno postave na vrijednost 0. U najgorem
	 * slucaju polje mora biti velicine 2^n*/
	
	stablo = (int *) malloc ((pow (2, n)) * sizeof (int));
	
	while (fscanf (in, "%d", &x) != EOF)
		dodaj_cvor (x, 1, stablo);

	inorder (1, stablo);
	
	return 0;
}
