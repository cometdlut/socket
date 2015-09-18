
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "type.h"
#include "socket.h"
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

int main(int argc, char *argv[]) {

	STATUS result;
	int sock;

	if(argc < 2) {

		printf("No port parameter.\n");
		exit(1);
	}

	// init cloud

	cloud_init();

	// register call back

	register_callback_func(is_connected, is_close, is_read);

	// new socket

	result = create_socket(&sock, atoi(argv[1]));
	assert(result == TRUE);

	//  run cloud

	cloud_run(sock);

	return 0;
}
