/* two_pipes.c - duplex communication using pipes
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_LENGTH 10

int main(int argc, char **argv)
{
	pid_t pid;
	char buffer[BUFFER_LENGTH];
	int operand1, operand2, operacija, num, tmp, fds_in[2], fds_out[2];
	int tocno, odgovor;
	
	if (argc != 2) {
		fprintf(stderr, "Sintaksa: a.out broj_upita\n");
		exit(EXIT_SUCCESS);
	}

	if (sscanf (argv[1], "%d", &num) != 1) {
		fprintf(stderr, "Sintaksa: a.out broj_upita\n");
		exit(EXIT_FAILURE);
	}
	
	while (num--) {	
		srand((unsigned) time (NULL));
	
		if (pipe(fds_in) == -1 || pipe (fds_out) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	
		if ((pid = fork ()) == -1 ) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
	
		if (pid == 0) {
			close(fds_in[1]);
			close(fds_out[0]);
			
			dup2(fds_in[0], STDIN_FILENO);
			dup2(fds_out[1], STDOUT_FILENO);

			if (execlp("bc", "bc", NULL) == -1) {
				perror("bc");
				exit(EXIT_FAILURE);
			}
		
			close (fds_out[1]);
		} else {
			close(fds_in[0]);
			close(fds_out[1]);

			operand1 = rand() % 99 + 1;
			operand2 = rand() % 99 + 1;
			operacija = rand() % 4;
	
			switch (operacija) {
				case 0:
					sprintf(buffer, "%d+%d\n", operand1, operand2);
					break;
			
				case 1:
					sprintf(buffer, "%d-%d\n", operand1, operand2);
					break;
			
				case 2:
					sprintf(buffer, "%d*%d\n", operand1, operand2);
					break;

				case 3:
					sprintf(buffer, "%d/%d\n", operand1, operand2);
					break;
			}

			write(STDOUT_FILENO, buffer, strlen (buffer) - 1);
	
			printf(" = ");
			fflush(stdout);
			scanf("%d", &odgovor);
		
			write(fds_in[1], buffer, strlen (buffer));
			close(fds_in[1]);

			wait(NULL);
	
			tmp = read(fds_out[0], buffer, BUFFER_LENGTH);
			buffer[tmp] = '\0';

			sscanf(buffer, "%d", &tocno);

			if (tocno == odgovor)
				printf ("TOCNO\n\n");
			else
				printf ("NETOCNO [Tocan odgovor = %d]\n\n", tocno);
		}
	}
		
	return EXIT_SUCCESS;
}
