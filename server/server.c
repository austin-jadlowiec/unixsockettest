/*
 * File server.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "sock_cfg.h"

int main(void) {
	struct sockaddr_un addr;
	int sock;
	int data_socket;
	int ret;
	int leave = 0;
	char buffer[BUFFER_SIZE];

	/* Create local socket */
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("unable to open socket");
		exit(EXIT_FAILURE);
	}
	printf("socket created\n");

	/* Clear structure for portability */
	memset(&addr, 0, sizeof(addr));

	/* Bind socket to socket name */
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_ADDR, sizeof(addr.sun_path));

	/* Unlink possible existing socket */
	unlink(SOCKET_ADDR);
	ret = bind(sock, (const struct sockaddr *) &addr, sizeof(addr));
	if (ret == -1) {
		perror("unable to bind socket");
		exit(EXIT_FAILURE);
	}

	/* Prepare for incoming connections */
	ret = listen(sock, MAX_REQ);
	if (ret == -1) {
		perror("unable to listen on socket");
		exit(EXIT_FAILURE);
	}
	printf("listening for incoming connections\n");

	for (;;) {
		/* Wait for incoming connections */
		data_socket = accept(sock, NULL, NULL);
		if (data_socket == -1) {
			perror("unable to accept connection");
			exit(EXIT_FAILURE);
		}
		printf("request started\n");

		for (;;) {
			/* Wait for next data packet */
			ret = read(data_socket, buffer, sizeof(buffer));
			if (ret == -1) {
				perror("unable to read from data socket. dc");
				leave = 1;
				break;
			}
			
			/* Ensure Null-termination */
			buffer[sizeof(buffer) - 1] = 0;

			printf("read: %s\n", buffer);

			/* Check for leave */
			if (!strncmp(buffer, "LEAVE", sizeof(buffer))) {
				leave = 1;
				break;
			}

			/* Check for end */
			if (!strncmp(buffer, "END", sizeof(buffer))) {
				break;
			}

			/* Echo it back */
			ret = write(data_socket, buffer, sizeof(buffer));
			if (ret == -1) {
				perror("unable to echo to data socket. dc");
				leave = 1;
				break;
			}
			printf("message echoed\n");
		}

		/* Close socket */
		close(data_socket);
		printf("closed request socket\n");

		if (leave) {
			break;
		}
	}

	close(sock);
	unlink(SOCKET_ADDR);
	printf("closed connection socket\n");

	exit(EXIT_SUCCESS);
}
