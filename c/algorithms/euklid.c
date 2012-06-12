/* euklid.c - trazenje najvece zajednicke mjere sa Euklidovim algoritmom
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * Euklidovo svojstvo:
 *     Nzm (u, v) = Nzm (max {u,v} mod min {u,v}, min {u,v})
 */

#include <stdio.h>

/* Rekurzivna implementacija */
int nzm (int u, int v)
{
	int tmp;
	
	if (u == 0)
		return v;

	if (u < v) {
		tmp = u;
		u = v;
		v = tmp;
	}

	return nzm (u % v, v);
}

int main ()
{
	int x, y;
	
	do {
		printf ("Unesi dva prirodna broja: ");
		scanf ("%d %d", &x, &y);
	} while (x<=0 || y<=0);

	printf ("Nzm (%d, %d) = %d\n", x, y, nzm (x,y));
			
	return 0;
}
