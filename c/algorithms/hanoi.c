/* hanoi.c - problem Hanojskih tornjeva
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * Problem:
 * Zadana su tri stapa (1, 2 i 3). Na stapu 1 nalazi se n diskova razlicite
 * velicine smjestenih tako da se manji disk nalazi uvijek iznad veceg diska.
 * Treba preseliti sve diskove sa stapa 1 na 3, jedan po jedan, tako da manji
 * disk uvijek dolazi na veci disk 
 * 
 * Opis algoritma:
 * 1. Ignorirati donji (najveci) disk i rijesiti problem za n-1 diskova
 *    sa stapa 1 na stap 2 koristeci stap 3 kao pomocni
 * 2. Najveci disk se nalazi na stapu 1, ostalih n-1 se nalazi na stapu 2
 * 3. Preseliti najveci disk sa 1 na 3
 * 4. Preseliti n-1 disk sa stapa 2 na stap 3 koristeci stap 1 kao pomocni
 */

#include <stdio.h>

/* Rekurzivno rjesenje problema. Ispisuje poredak potrebnih prebacivanja */
void hanoi (char src, char dest, char temp, int n)
{
	if (n > 0) {
		hanoi (src, temp, dest, n-1);
		printf ("%c --> %c\n", src, dest);
		hanoi (temp, dest, src, n-1);
	}
}

int main ()
{
	int n;

	printf ("Unesi broj diskova: ");
	scanf ("%d", &n);
	
	hanoi ('S', 'D', 'T', n); 
	
	return 0;
}

