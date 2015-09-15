
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "type.h"
#include "epoll.h"
#include "socket.h"
#include "sig.h"

// entry of executive file

int main(int argc, char *argv[])
{

	STATUS result;
	int sock;
	u32 ip;
	u16 port;

	// init all module

	epoll_init();

	sig_init();

	init_debug();


	// new socket

	result = create_socket(&sock);
	assert(result == TRUE);

	// bind socket

	ip = (127 << 24) | 0x1;
	port = 0x1234;
	result = bind_socket(sock, ip, port);
	assert(result == TRUE);

	// add socket to epoll

	epoll_add_socket(sock);

	// dead loop run

	epoll_run(sock);

	// return from while(1) procedure

	close(sock);

	return 0;
}
