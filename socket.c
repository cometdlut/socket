
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create socket

STATUS create_socket(int* socket) {

	int fd;

	fd = socket(PF_INET, SOCKET_STREAM, 0);
	if(fd < 0) {
	
		return FALSE;
	}

	*socket = fd;
	return TRUE;
}

// bind socket

STATUS bind_socket(int socket, u32 ip, u16 port) {

	struct sockaddr_in server_addr;
	int opt;
	int fd;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = ip;
	server_addr.sin_port = port;

	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if(bind(socket, (struct sockaddr*)(&server_addr), sizeof(server_addr))) {
	
		return FALSE;
	}

	return TRUE;
}

// listen socket

STATUS listen_socket(int socket, u32 queue) {

	if(listen(socket, queue)) {
	
		return FALSE;
	}

	return TRUE;

}

// accept socket

STATUS accept_socket(int socket, int* new_socket, u32* ip, u16* port){

	int fd;
	struct sockaddr_in client_addr;

	if(!new_socket || !ip || !port) {

		return FALSE;
	}

	fd = accept(socket, (struct sockaddr*)(&client_addr), sizeof(client_addr));
	if(fd < 0) {

		return FALSE;
	}

	*new_socket = fd;
	*ip = client_addr.sin_addr.s_addr;
	*port = client_addr.sin_port;

	return TRUE;
	
}

// read data

ListNode* read_socket(int socket) {

	return NULL;
}

// write data

void write_socket(int socket, char* buffer, int length){

	return;
}


