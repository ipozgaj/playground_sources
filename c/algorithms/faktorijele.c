/* faktorijele.c - rekurzivno racunanje faktorijela
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * n! = n * (n-1)!
 */

#include <stdio.h>

int fact (int n)
{
	if (n == 0)
		return 1;
	else
		return n * fact (n-1);
}

int main ()
{
	int x;
	
	do {
		printf ("Unesi prirodni broj: ");
		scanf ("%d", &x);
	} while (x<0);

	printf ("%d! = %d\n", x, fact (x));
		
	return 0;
}
