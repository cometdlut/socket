/*
 * Copyright 2015 tristan_fei <feixiaoxing@163.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "type.h"
#include "log.h"
#include "handle.h"
#include "buf.h"
#include "epoll.h"
#include "socket.h"

// create socket

STATUS create_socket(int *fd, u16 port) {

	int sock;
	struct sockaddr_in server_addr;
	int opt;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {

		log_print_error("Failed to create socket!\n");
		return FALSE;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(port);

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(sock, (struct sockaddr *)(&server_addr), sizeof(server_addr))) {

		log_print_error("Failed to bind socket!\n");
		return FALSE;
	}

	if (listen(sock, 5)) {

		log_print_error("Failed to listen socket!\n");
		return FALSE;
	}

	epoll_add_socket(sock);
	*fd = sock;

	return TRUE;

}

// accept socket

STATUS accept_socket(int sock, int *new_sock, u32 * ip, u16 * port) {

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

	epoll_add_socket(fd);

	*new_sock = fd;
	*ip = client_addr.sin_addr.s_addr;
	*port = client_addr.sin_port;

	return TRUE;

}

// forward message

void forward_sock_buffer(int sock, char* buf, int len) {

	RCV_BUF* p_rcv;

	p_rcv = (RCV_BUF*) malloc(sizeof(RCV_BUF));
	if(!p_rcv) {

		assert(0);
	}

	init_rcv_buf(p_rcv, buf, len);
	add_buf_to_sock(sock, p_rcv);
}

// remove socket

void remove_socket(int sock) {

	SEND_BUF* p_buf;
	SEND_SOCK* p_send;

	epoll_del_socket(sock);

	// delete send buf

	while(p_buf = get_send_buf(sock)) {

		free(p_buf-> buf);
		delete_send_buf(p_buf);

		free(p_buf);
	}

	// delete send sock

	p_send = find_send_sock(sock);
	assert(p_send);

	delete_send_sock(p_send);
	free(p_send);

	close(sock);
}

// read data

STATUS read_socket(int sock) {

	char *buf;
	int len;
	int ret;
	int broken;
	SEND_SOCK* p_send;
	SEND_BUF* p_buf;

	buf = (char *)malloc(READ_BUF_SIZE);
	if (!buf) {
		return FALSE;
	}

	ret = 0;
	len = 0;
	broken = 0;

	while (1) {

		// check if buffer is full already

		if(ret == READ_BUF_SIZE) {
			forward_sock_buffer(sock, buf, ret);

			buf = (char *)malloc(READ_BUF_SIZE);
			if (!buf) {

				assert(0);
				return FALSE;
			}

			ret = 0;
		}

		// read data from socket

		len = read(sock, buf + ret, READ_BUF_SIZE - ret);
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
		remove_socket(sock);

		return FALSE;
	}

	if(!len) {
		free(buf);
		return TRUE;
	}

	forward_sock_buffer(sock, buf, ret);
	return TRUE;
}

// write data

STATUS write_socket(int sock) {

	char* buf;
	int len;
	int broken;
	int ret;
	int total;

	SEND_SOCK* p_send;
	SEND_BUF* p_buf;

	// check if we have send buffer already

	p_buf = get_send_buf(sock);
	if(!p_buf) {

		return TRUE;
	}

	// initialise temp variable

	ret = 0;
	broken = 0;

	buf = p_buf-> buf + p_buf-> start;
	total = p_buf->len - p_buf-> start;

	while (1) {

		// check if all data has been sent

		if(ret == total ) {

			free(p_buf-> buf);
			delete_send_buf(p_buf);

			p_buf = get_send_buf(sock);
			if(!p_buf) {

				return TRUE;
			}

			ret = 0;
			buf = p_buf-> buf;
			total = p_buf-> len;
		}

		// begin to send data using socket api

		len = write(sock, buf + ret, len - ret);
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

	// if broken, just report it

	if (broken) {

		remove_socket(sock);
		return FALSE;
	}


	p_buf->start += ret;
	return TRUE;
}
