
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

// input 127.0.0.1 as address, 1234 as default port

int main(int argc, char* argv[]) {

	int sockfd;
	int numbytes;

	char buf[100];
	char* msg = "hello, world";
	struct sockaddr_in their_addr;

	if(argc < 2) {

		printf("No ip and port info.\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd) {

		printf("Failed to create socket. \n");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_addr.s_addr = inet_addr(argv[1]);
	their_addr.sin_port = htons(atoi(argv[2]));
	bzero(&their_addr.sin_zero, 8);

	if(-1 == connect(sockfd, (struct sockaddr*)&their_addr, sizeof(struct sockaddr))){

		printf("Cannot connect. \n");
		exit(1);
	}

	if(-1 == send(sockfd, msg, strlen(msg), 0)) {

		printf("Failed to send. \n");
		goto fail;
	}

	numbytes = recv(sockfd, buf, 100, 0);
	if(-1 == numbytes) {

		printf("Failed to receive. \n");
		goto fail;
	}

	buf[numbytes] = '\0';
	printf("%s\n", buf);

fail:
	close(sockfd);

	return 0;
}




