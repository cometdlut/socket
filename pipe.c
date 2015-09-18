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
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include "type.h"
#include "pipe.h"
#include "log.h"
#include "epoll.h"

static int pipe_ch[2];

// init pipe module

void init_pipe() {

	if(pipe(pipe_ch) < 0) {

		log_print_error("Failed to create pipe!\n");
	}

	socketpair(PF_UNIX, SOCK_STREAM, 0, pipe_ch);
	epoll_add_pipe(pipe_ch[1]);
}

// write pipe

void write_pipe(s8* buf, u32 len) {

	write(pipe_ch[0], buf, len);
}

// read pipe

void read_pipe(s8* buf, u32 len) {

	read(pipe_ch[1], buf, len);
}

// is pipe fd

STATUS is_pipe_fd(int fd) {

	return (fd == pipe_ch[1]) ? TRUE : FALSE;
}

// close pipe

void close_pipe() {

	close(pipe_ch[0]);
	close(pipe_ch[1]);
}

