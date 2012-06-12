/* lista.c - formira i ispisuje jednostruko povezanu listu od podataka 
 *           koji se nalaze u datoteci "ulaz.dat". Podaci su cjelobrojni 
 *           brojevi odvojeni razmakom.
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>

struct node {
	int key;
	struct node *next;
};

/* Inicijaliziraj praznu listu */
void init_list (struct node **head)
{
	*head = (struct node *) malloc (sizeof (struct node));
}

/* Umetni cvor iza zadanog cvora */
void insert_after (struct node **t, int key)
{
	struct node *new_node;

	new_node = (struct node *) malloc (sizeof (struct node));
	
	new_node->key = key;
	new_node->next = (*t)->next;
	(*t)->next = new_node;
}

/* Izbrisi sljedeci cvor */
void delete_next (struct node **t)
{
	struct node *p;
		
	p = (*t)->next;
	(*t)->next = (*t)->next->next;

	free (p);
}

/* Ispisi sve elemente liste pocevsi od cvora head */
void print_list (struct node *head)
{
	struct node *t = head;

	while (t->next != NULL) {
		t = t->next;
		printf ("%d ", t->key);
	}
}

int main ()
{
	int n;
	FILE *in;
	struct node *head;
	
	/* Unosi podatke za listu iz datoteke */
	if ((in = fopen ("ulaz.dat", "r")) == NULL) {
		fprintf (stderr, "No input file\n");
		exit (0);
	}
	
	init_list (&head);
	
	while ((fscanf (in, "%d", &n)) != EOF)
		insert_after (&head, n);
	
	print_list (head);
	
	return 0;
}
