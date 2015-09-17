
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "type.h"
#include "sig.h"
#include "log.h"
#include "pipe.h"

static void stub(int no) {

	return;
}

static void process(int no){

	write_pipe("ok", 2);
}

void sig_init() {

	signal(SIGINT, stub);
	signal(SIGALRM, process);
}


