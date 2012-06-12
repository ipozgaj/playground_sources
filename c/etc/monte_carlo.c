#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
	int i, hits, n = 100000;

	srand((unsigned) time(NULL));

	for (hits = 0, i = 0 ; i < n ; ++i)
		if ((pow((float)rand() / RAND_MAX, 2) + pow((float)rand() / RAND_MAX, 2)) < 1)
			++hits;

	printf ("%lf\n", 4. * hits / n);

	return 0;
}
