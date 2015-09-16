
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "type.h"
#include "epoll.h"
#include "socket.h"
#include "sig.h"
#include "handle.h"

// global variable

static int g_connect = 0;
static int g_close = 0;
static int g_read = 0;

// on connect

void is_connected(SOCK_HANDLE* p_hand) {

	g_connect ++;
}

// on close

void is_close(SOCK_HANDLE* p_hand) {

	g_close ++;
}

// on read

void is_read(SOCK_HANDLE* p_hand) {

	g_read ++;
}

// entry of executive file

int main(int argc, char *argv[])
{

	STATUS result;
	int sock;
	s8* ip;
	u16 port;

	// init all module

	epoll_init();
	sig_init();
	init_debug();


	// new socket

	ip = "127.0.0.1";
	port = 0x1234;

	result = create_socket(&sock, ip, port);
	assert(result == TRUE);

	// dead loop run

	epoll_run(sock);

	// return from while(1) procedure

	close(sock);

	return 0;
}
