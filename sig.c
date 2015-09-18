
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "type.h"
#include "sig.h"
#include "log.h"
#include "pipe.h"

static void stub(int no) {

	return;
}

static void process(int no) {

	write_pipe("", 1);
}

void sig_init() {

	pthread_t id;

	signal(SIGINT, stub);

	signal(SIGALRM, process);
}


