
#ifndef _HANDLE_H
#define _HANDLE_H

#include "list.h"

// define basic struct

typedef struct _SOCK_HANDLE SOCK_HANDLE;
typedef void(*func)(SOCK_HANDLE*);

struct _SOCK_HANDLE {

	ListNode node;
	ListNode read;

	int sock;
	func onConnect;
	func onClose;
	func onRecv;

};

// message type


#define NEW_SOCK   0x1
#define SOCK_CLOSE 0x2
#define RECV_DATA  0x3
#define TIME_OUT   0x4

// function prototype

void init_handle();
void register_callback_func(func, func, func);
void process_message(int, int);
SOCK_HANDLE* find_handle(int sock);

#endif


