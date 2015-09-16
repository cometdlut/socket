
#ifndef _SOCKET_H
#define _SOCKET_H

STATUS create_socket(int *sock);
STATUS bind_socket(int sock, s8* ip, u16 port);
STATUS listen_socket(int sock);
STATUS read_socket(int sock);
STATUS write_socket(int sock);

#endif
