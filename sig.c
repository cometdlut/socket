
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

static void* process(void* arg){

	while(1) {

		sleep(1);
		write_pipe("ok", 2);
	}

	return (void*)0;
}

void sig_init() {

	pthread_t id;

	signal(SIGINT, stub);

	pthread_create(&id, NULL, process, NULL);
}


