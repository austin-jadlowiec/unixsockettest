/*
 * File client.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "sock_cfg.h"

int main(int argc, char *argv[]) {
	struct sockaddr_un addr;
	int ret;
	int data_socket;
	int i;
	char buffer[BUFFER_SIZE];
	
	/* Create local socket */
	data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if (data_socket == -1) {
	   perror("unable to open socket");
	   exit(EXIT_FAILURE);
	}
	printf("socket opened\n");

	/* For portability, clear the structure */
	memset(&addr, 0, sizeof(addr));

	/* Connect socket to socket address. */
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_ADDR, sizeof(addr.sun_path));

	ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(addr));
	if (ret == -1) {
		perror("unable to connect to server");
		exit(EXIT_FAILURE);
	}
	printf("socket connected\n");

	for (i = 1; i < argc; ++i) {
		/* Send arg */
		ret = write(data_socket, argv[i], strlen(argv[i]) + 1);
		if (ret == -1) {
			perror("unable to write to socket");
			break;
		}

		/* Read result */
		ret = read(data_socket, buffer, sizeof(buffer));
		if (ret == -1) {
			perror("unable to read response");
			break;
		}

		/* Ensure buffer is 0-terminated */
		buffer[sizeof(buffer) - 1] = 0;

		/* Print result */
		printf("read (%d): %s\n", i, buffer);
	}

	/* Send end */
	ret = write(data_socket, "END\0", 4);
	if (ret == -1) {
		perror("unable to send END command to server");
	}

	/* Close socket */
	close(data_socket);
	printf("sock closed\n");

	exit(EXIT_SUCCESS);
}
