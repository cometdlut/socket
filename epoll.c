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
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "type.h"
#include "socket.h"
#include "handle.h"
#include "buf.h"
#include "define.h"

static int epoll;
static struct epoll_event events[MAX_EVENT_NUM];

void epoll_init() {

	epoll = epoll_create(5);

	memset(events, 0, sizeof(events));
}

void epoll_add_pipe(int fd) {

	int opt;

	// add socket to epoll

	struct epoll_event event;

	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event);

	//set non block mode

	opt = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, opt | O_NONBLOCK);

}


void epoll_add_socket(int sock) {

	int opt;

	// add socket to epoll

	struct epoll_event event;

	event.data.fd = sock;
	event.events = EPOLLIN;
	epoll_ctl(epoll, EPOLL_CTL_ADD, sock, &event);

	//set non block mode

	opt = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, opt | O_NONBLOCK);

}

void epoll_del_socket(int sock) {

	struct epoll_event event;
	event.data.fd = sock;

	epoll_ctl(epoll, EPOLL_CTL_DEL, sock, &event);
}

void epoll_mod_socket(int sock) {

	struct epoll_event event;

	event.data.fd = sock;
	event.events = EPOLLIN | EPOLLOUT;
	epoll_ctl(epoll, EPOLL_CTL_MOD, sock, &event);
}

void epoll_run(int listenfd) {

	int i;
	int size;
	int sock;
	int client;

	u32 ip;
	u16 port;
	SEND_SOCK* p_send;

	s8 buf[2];
	u32 len;

	while (1) {

		size = epoll_wait(epoll, events, MAX_EVENT_NUM, -1);
		if(size < 0) {

			if(errno == EINTR) {

				continue;
			}

			break;
		}

		for(i = 0; i < size; i ++) {

			sock = events[i].data.fd;

			// listen socket

			if(sock == listenfd) {

				// add socket to epoll procedure

				assert(TRUE == accept_socket(sock, &client, &ip, &port));
				epoll_add_socket(client);

				p_send = (SEND_SOCK*) malloc(sizeof(SEND_SOCK));
				if(!p_send) {

					assert(0);
				}

				// add socket to send buffer procedure

				init_send_sock(p_send, client);
				add_send_sock(p_send);

				// send message for after use

				process_message(NEW_SOCK, client);
			}

			// check if it is pipe

			else if(TRUE == is_pipe_fd(events[i].data.fd)) {

				read_pipe(buf, 1);
				update_timer();
			}

			// check if socket close already

			else if(events[i].events & EPOLLRDHUP) {

				remove_socket(sock);
				process_message(SOCK_CLOSE, sock);
			}

			// read data

			else if(events[i].events & EPOLLIN) {

				if(TRUE != read_socket(sock)) {

					process_message(SOCK_CLOSE, sock);
				}
			}

			// send data

			else if(events[i].events & EPOLLOUT) {

				if(TRUE != write_socket(sock)) {

					process_message(SOCK_CLOSE, sock);
				}
			}
		}
	}
}

