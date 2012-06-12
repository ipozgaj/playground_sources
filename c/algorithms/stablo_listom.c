/* stablo_listom - implementacija binarnog stabla pomocu liste. 
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 * 
 * Podaci za cvorove se unose iz datoteke "ulaz.dat", zatim se formira 
 * stablo, te ispisu podaci koristeci inorder, preorder i postorder prolazak 
 * kroz stablo. Podaci su cjelobrojni brojevi odvojeni razmakom.
 */

#include <stdio.h>
#include <stdlib.h>

struct node_t {
	int key;
	struct node_t *left, *right;
};

typedef struct node_t node;

node *dodaj_cvor (node *root, int key)
{
	if (root == NULL) {
		root = (node *) malloc (sizeof (node));
		if (root) {
			root->key = key;
			root->left = root->right = NULL;
		}
	} else {
		if (key < root->key) {
			root->left = dodaj_cvor (root->left, key);
		} else if (key > root->key) {
			root->right = dodaj_cvor (root->right, key);
		} else {
			printf ("Podatak vec postoji\n");
		}
	}

	return root;
}

void inorder (node *root)
{
	if (root) {
		inorder (root->left);
		printf ("%d ", root->key);
		inorder (root->right);
	}
}

void postorder (node *root)
{
	if (root) {
		postorder (root->left);
		postorder (root->right);
		printf ("%d ", root->key);
	}
}

void preorder (node *root)
{
	if (root) {
		printf ("%d ", root->key);
		preorder (root->left);
		preorder (root->right);
	}
}

int main ()
{
	FILE *in;
	int x;
	node *root = NULL;

	if ((in = fopen ("ulaz.dat", "r")) == NULL) {
		fprintf (stderr, "Ulazna datoteka ne postoji\n");
		exit (EXIT_FAILURE);
	}

	while (fscanf (in, "%d", &x) != EOF)
		root = dodaj_cvor (root, x);

	printf ("Preorder prolaz: ");
	preorder (root);
	printf ("\nInorder prolaz: ");
	inorder (root);
	printf ("\nPostorder prolaz: ");
	postorder (root);
	printf ("\n");

	return 0;
}
