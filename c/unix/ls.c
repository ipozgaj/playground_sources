/* ls.c - list files in current directory
 *
 * (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

void list_dir(char *initial_dir)
{
	DIR *current_dir;
	struct dirent *current_entry;
	
	if ((current_dir = opendir (initial_dir)) == NULL) {
		perror(initial_dir);
		exit(EXIT_FAILURE);
	}
	
	while ((current_entry = readdir(current_dir)) != NULL) {
		printf("Inode: %d\t", current_entry->d_ino);
		printf("Length: %u\t", current_entry->d_reclen);
		
		switch (current_entry->d_type) {
			case 4: 
				printf("Type: dir\t");
				break;
			case 8: 
				printf("Type: file\t");
				break;
		}

		printf("%s\n", current_entry->d_name);
	}
	
	printf("\n");
}

int main(int argc, char **argv)
{
	char *initial_dir;

	if (argc == 1) 
		initial_dir = (char *) get_current_dir_name ();
	else
		initial_dir = strdup (argv[1]);
	
	list_dir(initial_dir);
	
	return EXIT_SUCCESS;
}
