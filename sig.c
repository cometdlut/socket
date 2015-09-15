
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "type.h"
#include "sig.h"
#include "log.h"

void func(int no){

	log_print_msg("Got SIGINT message in signal file!\n ");
	return;
}

void sig_init() {

	signal(SIGINT, func);
}


