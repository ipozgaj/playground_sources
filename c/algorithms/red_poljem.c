/* red_poljem.c - implementacija reda pomocu ciklickog polja
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>

#define QUEUE_SIZE 100

/* Stavi podatak u red */
int put (int val, int *queue, int *in, int out)
{
	/* Da li je red pun */
	if ((*in + 1) % QUEUE_SIZE == out) return 0;
	
	(*in)++;
	*in %= QUEUE_SIZE;
	queue[*in] = val;

	return 1;
}

/* Uzmi podatak iz reda */
int get (int *val, int *queue, int in, int *out)
{
	/* Da li je red prazan */
	if (in == *out) return 0;

	(*out)++;
	*out %= QUEUE_SIZE;
	*val = queue[*out];
	
	return 1;
}

int main ()
{
	int in, out, x;
	int queue[QUEUE_SIZE];
	
	in = out = 0;

	/* Primjer koristenja */
	put (1, queue, &in, out);
	put (2, queue, &in, out);
	get (&x, queue, in, &out);
	printf ("%d\n", x);
	put (3, queue, &in, out);
	get (&x, queue, in, &out);
	printf ("%d\n", x);
	get (&x, queue, in, &out);
	printf ("%d\n", x);
	
	return 0;
}

