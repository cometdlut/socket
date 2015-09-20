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
#include <assert.h>

#include "type.h"
#include "socket.h"
#include "handle.h"

// global variable

static int g_connect = 0;
static int g_close = 0;
static int g_read = 0;

// on connect

void is_connected(RCV_SOCK* p_hand) {

	g_connect ++;
}

// on close

void is_close(RCV_SOCK* p_hand) {

	g_close ++;
}

// on read

void is_read(RCV_SOCK* p_hand) {

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
