#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080

int main(int argc, char *argv[]) {
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) {
		printf("Failed to create client socket\n");
		return -1;
	}

	char buffer[1024] = { 0 };

	struct sockaddr_in serverAddr = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT)
	};

	int len = sizeof(serverAddr);

	if (connect(client_fd, (struct sockaddr*) &serverAddr, len) < 0) {
		printf("Connection Failed\n");
		close(client_fd);
		exit(0);
	}

	printf("Concect to server [%s : %d] success\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	while (1) {
		printf("Please input the message:");
		scanf("%s", buffer);

		if (send(client_fd, buffer, sizeof(buffer), 0) < 0) {
			printf("Failed to send data\n");
			memset(buffer, 0, sizeof(buffer));
			return -1;
		}

		if (strcmp(buffer, "exit") == 0) {
			break;
		}

		memset(buffer, 0, sizeof(buffer));

		if (recv(client_fd, buffer, sizeof(buffer), 0) < 0) {
			printf("Faield to recieve the data\n");
			return -1;
		}
	}

	if (close(client_fd) < 0) {
		printf("Failed to close the client socket\n");
		return -1;
	}

	return 0;
}
