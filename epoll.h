
#ifndef _EPOLL_H
#define _EPOLL_H

void epoll_init();
void epoll_add_socket(int);
void epoll_del_socket(int);
void epoll_run();

#endif
