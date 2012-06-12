/* fibbonaci.c - racunanje Fibbonacijevih brojeva
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * n-ti Fibbonacijev broj definiran je rekurzivnom formulom:
 *
 *    F(n) = F(n-1) + F(n-2)        za n>=3
 *
 *    F(1) = F(2) = 1
 *
 * Fibbonacijev niz:
 *
 *    1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, ...
 */

#include <stdio.h>

/* Rekurzivna implementacija */
int fibbonaci (int n)
{
	if (n<=2)
		return 1;
	 
	return fibbonaci (n-1) + fibbonaci (n-2);
}

/* Iterativna implementacija */
int fibbonaci_i (int n)
{
	int i, f1, f2, fibb;

	f1 = f2 = fibb = 1;
	i=2;
	
	while (++i<=n) {
		fibb = f1 + f2;
		f2 = f1;
		f1 = fibb;
	}

	return fibb;
}


int main ()
{
	int x;

	do {
		printf ("Unesi prirodni broj: ");
		scanf ("%d", &x);
	} while (x<=0);

	printf ("Iterativno: F(%d) = %d\n", x, fibbonaci_i (x));
	printf ("Rekurzivno: F(%d) = %d\n", x, fibbonaci (x));
		
	return 0;
}

