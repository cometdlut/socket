
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

