
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "type.h"
#include "socket.h"
#include "handle.h"


void is_connected(SOCK_HANDLE* p_hand) {

	return;
}

// on close

void is_close(SOCK_HANDLE* p_hand) {

	return;
}

// on read

void is_read(SOCK_HANDLE* p_hand) {

	s8* buf;
	u32* len;
	STATUS result;
	
	result = get_rcv_buf(p_hand->sock, &buf, &len);
	assert(TRUE == result);

	send_buf(buf, len);
	free(buf);	
}

// entry of executive file

int main(int argc, char *argv[]) {

	STATUS result;
	int sock;

	if(argc < 2) {

		printf("No port parameter. \n");
		exit(1);
	}

	// init cloud

	cloud_init();

	// register call back

	register_callback_func(is_connected, is_close, is_read);

	// new socket

	result = create_server_socket(&sock, atoi(argv[1]));
	assert(result == TRUE);

	//  run cloud

	cloud_run(sock);

	return 0;
}
