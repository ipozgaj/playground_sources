/* kraljice.c - problem osam kraljica
 *
 * (C) 2004 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * Problem:
 * Smjestit osam kraljica na sahovsku plocu tako da se one medjusobno
 * ne napadaju. (Postoji tocno 92 razlicita rjesenja)
 *
 * Opis algoritma:
 * 1. Promatramo stupce od prvog prema zadnjem
 * 2. U svaki stupac postavljamo jednu kraljicu
 * 3. Promatramo plocu kada je vec postavljeno i kraljica (u i razlicitih
 *    stupaca) koje se medjusobno ne napadaju
 * 4. Postavljamo i+1 kraljicu tako da ona ne napada niti jednu od vec
 *    postavljenih kraljica i da se ostale kraljice mogu postaviti uz
 *    uvjet nenapadanja
 */

#include <stdio.h>

/* Ispituje da li se kraljice na pozicijama (x1, y1) i (x2, y2) napadaju.
 * Vraca 1 ako se napadaju, 0 ako se ne napadaju */
int atacked (int x1, int y1, int x2, int y2)
{
	int t = 0;

	if (x1 == x2) t = 1;
	if (y1 == y2) t = 1;
	if (x1 + y2 == x2 + y1) t = 1;
	if (x1 - y2 == x2 - y1) t = 1;

	return t;
}

int q8 (int *queens, int i, int n)
{
	int a, b;
	int good;

	if (i == n) return 1;

	for (a=0 ; a < n ; a++) {
		good = 1;
		
		for (b=0 ; b<i ; b++) {
			if (atacked (b+1, queens[b]+1, i+1, a+1)) {
				good = 0;
				break;
			}
		}

		if (good) {
			queens[i] = a;
			if (q8 (queens, i+1, n) == 1) return 1;
		}
	}
	
	return 0;
}

int main ()
{
	int i;
	int queens[8] = {0};

	q8 (queens, 0, 8);

	for (i=0 ; i<8 ; i++)
		printf ("(%d, %d)\n", i+1, queens[i]+1);

	return 0;
}

