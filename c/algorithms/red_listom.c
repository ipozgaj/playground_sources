/* red_listom.c - implementacija reda pomocu jednostruko povezane liste 
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>

struct node_t {
	int key;
	struct node_t *next;
};

typedef struct node_t node;

/* Stavi podatak u red */
void put (int val, node **head, node **tail)
{
	node *new_node;

	new_node = (node *) malloc (sizeof (node));
	
	new_node->key = val;
	new_node->next = NULL;
	
	if (*head == NULL) {
		*head = new_node;
	} else {
		(*tail)->next = new_node;
	}
	
	*tail = new_node;
}

/* Uzmi podatak iz reda */
int get (node **head, node **tail)
{
	int ret_val;
	node *tmp;

	if (*head == NULL) return -1;

	tmp = *head;
	ret_val = tmp->key;

	*head = (*head)->next;
	free (tmp);

	return ret_val;
}

int main ()
{
	node *head, *tail;
	
	head = tail = NULL;
	
	/* Primjer koristenja */
	put (1, &head, &tail);
	put (2, &head, &tail);
	printf ("%d\n", get (&head, &tail));
	put (3, &head, &tail);
	printf ("%d\n", get (&head, &tail));
	printf ("%d\n", get (&head, &tail));
	
	return 0;
}
