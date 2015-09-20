
#ifndef _HANDLE_H
#define _HANDLE_H

#include "list.h"

// define basic struct

typedef struct _RCV_SOCK RCV_SOCK;
typedef void(*func)(RCV_SOCK*);

typedef struct _RCV_BUF {

	ListNode node;
	s8* buf;
	u32 len;

}RCV_BUF;


struct _RCV_SOCK {

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
RCV_SOCK* find_handle(int sock);
STATUS get_rcv_buf(int sock, s8** buf, u32* len);

#endif


