
#ifndef _SOCKET_H
#define _SOCKET_H

#define READ_BUF_SIZE (1024)

STATUS create_socket(int *sock, s8* ip, u16 port);
STATUS read_socket(int sock);
STATUS write_socket(int sock);

#endif
