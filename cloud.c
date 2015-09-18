
#include "cloud.h"

void cloud_init() {

	// init all module, the procedure can not be reordered

	init_debug();

	epoll_init();

	init_pipe();

	init_timer();

	sig_init();

	init_buffer();
}

void cloud_run(int sock) {

	// dead loop run

	epoll_run(sock);

	// return from while(1) procedure

	close_pipe();
	close(sock);

}
