
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "type.h"
#include "log.h"
#include "handle.h"

// create socket

STATUS create_socket(int *sock)
{

	int fd;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {

		log_print_error("Failed to create socket!\n");
		return FALSE;
	}

	*sock = fd;
	return TRUE;
}

// bind socket

STATUS bind_socket(int sock, u32 ip, u16 port)
{

	struct sockaddr_in server_addr;
	int opt;
	int fd;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = ip;
	server_addr.sin_port = port;

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(sock, (struct sockaddr *)(&server_addr), sizeof(server_addr))) {

		log_print_error("Failed to bind socket!\n");
		return FALSE;
	}

	return TRUE;
}

// listen socket

STATUS listen_socket(int sock)
{

	if (listen(sock, 5)) {

		log_print_error("Failed to listen socket!\n");
		return FALSE;
	}

	return TRUE;

}

// accept socket

STATUS accept_socket(int sock, int *new_sock, u32 * ip, u16 * port)
{

	int fd;
	struct sockaddr_in client_addr;
	int client_len;

	if (!new_sock || !ip || !port) {

		log_print_error("Parameter is NULL in accept_socket function!\n");
		return FALSE;
	}

	client_len = sizeof(client_addr);
	fd = accept(sock, (struct sockaddr *)(&client_addr), &client_len);
	if (fd < 0) {

		log_print_error("Failed to accepet socket!\n");
		return FALSE;
	}

	*new_sock = fd;
	*ip = client_addr.sin_addr.s_addr;
	*port = client_addr.sin_port;

	return TRUE;

}

// forward message

void forward_sock_buffer(int sock, char* buf, int length) {

	return;
}

// read data

STATUS read_socket(int sock)
{

	char *buf;
	int len;
	int ret;
	int broken;

	buf = (char *)malloc(1024);
	if (!buf) {
		return FALSE;
	}

	ret = 0;
	len = 0;
	broken = 0;

	while (1) {

		len = read(sock, buf + ret, 1024 - ret);
		if (len == -1) {
			if (errno == EINTR) {
				continue;
			} else {
				broken = 1;
				break;
			}
		} else if (len == 0) {
			break;
		} else {

			ret += len;
		}
	}

	if (broken) {

		free(buf);
		close(sock);

		process_message(SOCK_CLOSE, sock);
		return FALSE;
	}

	if(!len) {
		free(buf);
		return TRUE;
	}

	forward_sock_buffer(sock, buf, len);
	return TRUE;
}

// write data

STATUS write_socket(int sock)
{

	char* buf;
	int length;

	int len;
	int broken;
	int ret;

	ret = 0;
	broken = 0;

	while (1) {

		len = write(sock, buf + ret, length - ret);
		if (len == -1) {
			if (errno == EINTR) {
				continue;
			} else {
				broken = 1;
				break;
			}
		} else if (len == 0) {
			break;
		} else {
			ret += len;
		}
	}

	if (broken) {

		free(buf);
		return FALSE;
	}

	return TRUE;
}
