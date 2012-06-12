/* socket_inet.c - get web page content
 *
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1000

void get_homepage(int socket_fd)
{
	char buffer[BUFFER_SIZE];
	ssize_t characters_read_num;

	sprintf(buffer, "GET /\n");
	write(socket_fd, buffer, strlen(buffer));

	while (1) {
		characters_read_num = read(socket_fd, buffer, BUFFER_SIZE);

		if (characters_read_num == 0)
			return;

		fwrite(buffer, sizeof (char), characters_read_num, stdout);
	}
}


int main (int argc, char **argv)
{
	int socket_fd;
	struct sockaddr_in name;
	struct hostent *hostinfo;

	if ((hostinfo = gethostbyname(argv[1])) == NULL)
		return 1;

	name.sin_family = AF_INET;
	name.sin_addr = *((struct in_addr*) hostinfo->h_addr);
	name.sin_port = htons(80);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(socket_fd, &name, sizeof (struct sockaddr_in)) == -1) {
		perror("connect");
		return 1;
	}

	get_homepage(socket_fd);

	return 0;
}
