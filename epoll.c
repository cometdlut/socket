
#include <stdio.h>
#include <stdlib.h>

static int epoll;

void epoll_init() {
	
	epoll = epoll_create(5);
}

void epoll_add_socket(int socket) {
	
	epoll_ctl();
}

void epoll_del_socket(int socket) {
	
	epoll_ctl();
}

void epoll_run() {
	
	while(1) {

		epoll_wait();

	}
	
}
