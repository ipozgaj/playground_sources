/* pow.c - racunanjem potencija primjenom rekurzije
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>

/* Rekurzivno racunanje */
int power (int x, int y)
{
	if (y==0)
		return 1;
	
	return x * power (x, y-1);
}

/* Iterativno racunanje */
int power_i (int x, int y)
{
	int i, p;

	if (y==0)
		return 1;
	
	for (p=1, i=1 ; i<=y ; i++)
		p *= x;

	return p;
}

int main ()
{
	int x, y;

	do {
		printf ("Unesi dva cijela broja: ");
		scanf ("%d %d", &x, &y);
	} while (x<0 || y<0);

	printf ("Iterativno: %d^%d = %d\n", x, y, power_i (x, y));
	printf ("Rekurzivno: %d^%d = %d\n", x, y, power (x, y));
		
	return 0;
}

