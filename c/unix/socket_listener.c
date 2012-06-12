/* listener.c - simple network server program
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

const char *welcome_msg = "Welcome to simple network listener v1.0 \n(C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>\nType 'quit' or 'exit' to terminate connection\n";

int main(int argc, char **argv)
{
	int server_socket_fd, client_socket_fd, port;
	struct sockaddr_in server_address, client_address;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: server [port]\n");
		exit(EXIT_FAILURE);
	}
	
	sscanf(argv[1], "%d", &port);
	
	server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);
	bind(server_socket_fd, (struct sockaddr *) &server_address, sizeof (server_address));
	
	listen(server_socket_fd, 1);

	printf("Started server on port %d\n", port);

	while (1) {
		char buffer[256];
		int len, msg_len, client_active = 1;

		printf("Server waiting for connections...\n");

		len = sizeof (client_address);
		client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &client_address, &len);

		write(client_socket_fd, welcome_msg, strlen (welcome_msg));
		
		printf("==> Connection from: %s\n", inet_ntoa(client_address.sin_addr));
		
		while (client_active) {
			msg_len = read(client_socket_fd, buffer, 256);
			buffer[msg_len] =  '\0';
			if (msg_len > 0) printf("==> Read: %s", buffer);
	
			if (!strncmp (buffer, "quit", 4) || !strncmp (buffer, "exit", 4)) client_active = 0;
		}
		
		close(client_socket_fd);
	}
	
	return EXIT_SUCCESS;
}
