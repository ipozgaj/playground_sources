/* stog_listom.c - implementacija stoga pomocu jednostruko povezane liste
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node {
	int key;
	struct node *next;
};

void stack_init (struct node **head)
{
	*head = (struct node *) malloc (sizeof (struct node));
}

/* Stavi podatak na stog */
void push (struct node **head, int key)
{
	struct node *new_node;

	new_node = (struct node *) malloc (sizeof (struct node));
	new_node->key = key;
	new_node->next = (*head)->next;
	(*head)->next = new_node;
}

/* Uzmi podatak sa stoga */
int pop (struct node **head)
{
	int ret_val;
	struct node *tmp;
		
	/* Da li je stog prazan? Vrati -1 ako je */
	if ((*head)->next == NULL) return -1;

	tmp = (*head)->next;
	ret_val = tmp->key;
	(*head)->next = (*head)->next->next;
	free (tmp);

	return ret_val;
}

int main ()
{
	struct node *head;
	int val, ret_val;
	
	stack_init (&head);
	
	/* Generiraj pseudoslucajne brojeve. Ako je generiran parni broj
	 * stavi ga na stog, ako je neparan uzmi podataka sa stoga. Prekini
	 * ako je stog prazan. */

	/* Inicijaliziraj generator pseudoslucajnih brojeva */
	srand ((unsigned) time (NULL));

	while (1) {
		val = 1 + (int)(100.0 * rand() / (RAND_MAX + 1.0));
		
		/* Neparan */
		if (val % 2) {
			ret_val = pop (&head);
			
			if (ret_val == -1) {
				printf ("Stog je prazan\n");
				exit (0);
			}

			printf ("Pop = %d\n", ret_val);
		} else {
			/* Paran */
			push (&head, val);
			printf ("Push (%d)\n", val);
		}
	}

	return 0;
}

