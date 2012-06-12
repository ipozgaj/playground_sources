/* stog_datotekom - implementacija stoga pomocu datoteke
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Stavi novu vrijednost na stog. Vraca 1 ako je operacija uspjela, 
   inace vraca 0 */
int push (int val, FILE *stack, int stack_size, int *top)
{
	/* Provjeri da li je stog pun */
	if (*top > stack_size - 1)
		return 0;

	/* Stavi vrijednost na stog i inkrementiraj vrh stoga */
	fseek (stack, *top * sizeof (val), SEEK_SET);
	fwrite (&val, sizeof (val), 1, stack);

	(*top)++;

	return 1;
}

/* Uzmi vrijednost sa stoga. Vraca 1 ako je operacija uspjela, 
   inace vraca 0 */
int pop (int *val, FILE *stack, int *top)
{
	/* Provjeri da li je stog prazan */
	if (*top == 0) 
		return 0;

	/* Procitaj vrijednost sa stoga i dekrementiraj vrh stoga */
	(*top)--;
	fseek (stack, *top * sizeof (*val), SEEK_SET);
	fread (val, sizeof (*val), 1, stack);

	return 1;
}

int main ()
{
	FILE *stack;
	int top;
	int x, tmp, stack_size;

	do {
		printf ("Unesi velicinu stoga: ");
		scanf ("%d", &stack_size);
	} while (stack_size <= 0);

	/* Inicijaliziraj stog */
	if ((stack = fopen ("stack.dat", "w+b")) == NULL) {
		fprintf (stderr, "Ne mogu inicijalizirati stog\n");
		exit (1);
	};

	top = 0;
	
	/* Generiraj pseudoslucajne brojeve izmedju 1-100. Ako je generiran parni 
	 * broj, stavi ga na stog, ako je neparni, citaj sa stoga. Prekini 
	 * izvodjenje kada se pokusa citati sa praznog stoga */

	srand ((unsigned) time (NULL));
	
	while (1) {
		/* Ne racunati pomocu rand() % 100 + 1 !!! */
		x = (int) (100.0 * rand() / (RAND_MAX + 1.0)) + 1;

		/* Da li je broj neparan? */
		if (x%2) {
			if (!pop (&tmp, stack, &top)) {
				printf ("Stog prazan\n");
				exit (0);
			} else {
				printf ("pop => %d\n", tmp);
			}
		} else {
			if (!push (x, stack, stack_size, &top))
				printf ("Stog pun\n");
			else
				printf ("push (%d)\n", x);
		}
	}
	
	fclose (stack);
	
	return 0;
}

