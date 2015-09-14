
#ifndef _CONFIG_H
#define _CONFIG_H

// set epoll as default socket io model

#define EPOLL_SWITCH   0x1
#define SELECT_SWITCH  0x0
#define KQUEUE_SWITCH  0x0

#endif
