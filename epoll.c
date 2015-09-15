
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>

#include "type.h"
#include "socket.h"
#include "handle.h"

#define MAX_EVENT_NUM 1024

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

void epoll_run(int listenfd) {

	int i;
	int ret;
	int sock;
	u32 client;
	u32 ip;
	u16 port;
	STATUS result;
	s8* buf;

	while (1) {

		ret = epoll_wait(epoll, events, MAX_EVENT_NUM - 1, -1);
		if(ret < 0) {
			break;
		}

		for(i = 0; i < ret; i ++) {

			sock = events[i].data.fd;

			// listen socket

			if(sock == listenfd) {

				result = accept_socket(sock, &client, &ip, &port);
				epoll_add_socket(client);

				process_message(NEW_SOCK, client);
			}

			// read data

			else if(events[i].events & EPOLLIN) {

				read_socket(sock);
			}

			// send data

			else if(events[i].events & EPOLLOUT) {

				buf = "hello, world";
				write_socket(sock, buf, strlen(buf));
			}
		}
	}
}

