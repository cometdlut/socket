
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define MAX_EVENT_NUM 1024

static int epoll;
static struct epoll_event events[MAX_EVENT_NUM];

void epoll_init()
{
	epoll = epoll_create(5);
}

void epoll_add_socket(int sock)
{
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

void epoll_del_socket(int sock)
{

	struct epoll_event event;
	event.data.fd = sock;

	epoll_ctl(epoll, EPOLL_CTL_DEL, sock, &event);
}

void epoll_run()
{
	int i;
	int ret;
	int sock;

	while (1) {

		ret = epoll_wait(epoll, events, MAX_EVENT_NUM - 1, -1);
		if(ret < 0) {
			break;
		}

		for(i = 0; i < ret; i ++) {

			sock = events[i].data.fd;

			// listen socket

			if(0) {

				continue;
			}

			// read data

			else if(events[i].events & EPOLLIN) {

				continue;
			}

			// send data

			else if(events[i].events & EPOLLOUT) {

				continue;
			}

		}

	}

}
