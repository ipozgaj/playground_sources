#include <stdio.h>
#include <stdlib.h>

struct node_t {
	int id;
	struct node_t *next;
};

typedef struct node_t node;

node *reverse_list(node *head)
{
	node *new_head = NULL, *tmp, *p = head;

	while (p) {
		tmp = p->next;

		if (!new_head) {
			new_head = p;
		} else {
			p->next = new_head;
			new_head = p;
		}

		p = tmp;
	}

	return new_head;
}

int main()
{
	int id;
	node *head, *tail;
	node *reversed_head, *p, *tmp;

	head = tail = NULL;

	while (fscanf(stdin, "%d", &id) != EOF) {
		node *new_node = malloc(sizeof(node));
		new_node->id = id;
		new_node->next = NULL;
		if (!head) {
			head = tail = new_node;
		} else {
			tail->next = new_node;
			tail = new_node;
		}
	}

	head = reverse_list(head);

	return 0;
}
