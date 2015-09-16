
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "type.h"
#include "pipe.h"
#include "log.h"

static int pipe_ch[2];

// init pipe module

void init_pipe() {

	if(pipe(pipe_ch) < 0) {

		log_print_error("Failed to create pipe!\n");
	}
}

// write pipe

void write_pipe(s8* buf, u32 len) {

	write(pipe_ch[0], buf, len);
}

// read pipe

void read_pipe(s8* buf, u32 len) {

	read(pipe_ch[1], buf, len);
}

// close pipe

void close_pipe() {

	close(pipe_ch[0]);
	close(pipe_ch[1]);
}

