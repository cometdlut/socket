
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "type.h"
#include "config.h"
#include "log.h"

static u32 ip;
static u32 port;

void load_config() {

	s8 buf[1024];
	u32 len;

	// read file first

	FILE* stream;
	stream = fopen("base.config", "rw+");
	if(stream) {

		log_print_error("Failed to open config file. \n");
		return;
	}

	len = fread(buf, 1024, 1, stream);
	if(!len) {

		log_print_error("Failed to read config file. \n");
		return;
	}

	fclose(stream);

	// then parse data

	sscanf(buf, "%d:%d", &ip, &port);
}

u32 get_parse_ip() {

	return ip;
}

u16 get_parse_port() {

	return (u16)port;
}

