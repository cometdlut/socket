
#ifndef _EPOLL_H
#define _EPOLL_H

void epoll_init();
void epoll_add_socket();
void epoll_del_socket();
void epoll_run();

#endif

