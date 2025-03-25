#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main(int argc, char *argv[]) {

	char buffer[1024] = { 0 };

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		printf("Failed to create the server socket\n");
		return -1;
	}

	struct sockaddr_in serverAddr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(PORT)
	};

	if (bind(server_fd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0) {
		perror("Bind Failed");
		close(server_fd);
		exit(0);
	}

	if (listen(server_fd, 5) == -1) {
		printf("Error %d: Failed to listen\n", server_fd);
		close(server_fd);
		exit(0);
	}

	printf("Server [%s : %d] --- Ready\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	
	int reply_sockfd;
	struct sockaddr_in clientAddr;

	int client_len = sizeof(clientAddr);

	reply_sockfd = accept(server_fd, (struct sockaddr*) &clientAddr, &client_len);
	printf("Accepted connect request from [%s : %d]\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	while (recv(reply_sockfd, buffer, sizeof(buffer), 0)) {
		if (strcmp(buffer, "exit") == 0) {
			memset(buffer, 0, sizeof(buffer));
			exit(0);
		}
		
		char *msg = buffer;

		printf("Get message from [%s : %d]: ", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		printf("%s\n", msg);


		if (send(reply_sockfd, msg, sizeof(msg), 0) < 0) {
			printf("send data to [%s : %d] failed\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
			memset(buffer, 0, sizeof(buffer));
			exit(0);
		}
		
		memset(buffer, 0, sizeof(buffer));
	}

	if (close(reply_sockfd) < 0) {
		perror("Failed to close socket");
	}

	return 0;
}
