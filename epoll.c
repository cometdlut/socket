
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>
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
}

void epoll_add_socket(int sock) {

	int opt;

	// add socket to epoll

	struct epoll_event event;

	event.data.fd = sock;
	event.events = EPOLLIN;
	epoll_ctl(epoll, EPOLL_CTL_ADD, sock, &event);

	//set non block mode

	opt = fcntl(sock, F_GETFL);
	fcntl(sock, opt | O_NONBLOCK);

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

	while (1) {

		size = epoll_wait(epoll, events, MAX_EVENT_NUM - 1, -1);
		if(size < 0) {
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

