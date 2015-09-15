
#ifndef _HANDLE_H
#define _HANDLE_H

#include "list.h"

// define basic struct

typedef void(*func)(void*);

typedef struct _SOCK_HANDLE {

	ListNode node;
	ListNode read;
	ListNode write;

	int sock;
	func onConnect;
	func onClose;
	func onRecv;

}SOCK_HANDLE;

// message type

#define NEW_SOCK   0x1
#define SOCK_CLOSE 0x2
#define RECV_DATA  0x3
#define TIME_OUT   0x4

// function prototype

void init_handle();
void register_callback_func(func, func, func);
void process_message(int, int);

#endif


