
#ifndef _SOCKET_H
#define _SOCKET_H

#include "define.h"

STATUS create_socket(int *sock, u16 port);
STATUS read_socket(int sock);
STATUS write_socket(int sock);

#endif
