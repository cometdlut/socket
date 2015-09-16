
#include "cloud.h"

void cloud_init()
{
	// init all module

	epoll_init();

	sig_init();

	init_debug();

}

void cloud_run(int sock)
{
	// dead loop run

	epoll_run(sock);

	// return from while(1) procedure

	close(sock);

}
