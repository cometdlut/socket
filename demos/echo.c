
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
	s8* ip;
	u16 port;

	// init cloud

	cloud_init();

	// register call back

	register_callback_func(is_connected, is_close, is_read);

	// new socket

	ip = "127.0.0.1";
	port = 0x1234;

	result = create_socket(&sock, ip, port);
	assert(result == TRUE);

	//  run cloud

	cloud_run(sock);

	return 0;
}
