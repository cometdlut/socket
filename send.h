
#ifndef _SEND_H
#define _SEND_H

#include "list.h"

// define structure


typedef struct _SEND_BUF {
	
	ListNode node;
	s8* buf;
	u32 len;
	u32 start;
	
}SEND_BUF;

typedef struct _SEND_SOCK{
	
	ListNode node;
	int sock;
	ListNode head;
	
}SEND_SOCK;


// define function prototype

SEND_BUF* get_send_buf(int sock);
STATUS get_rcv_buf(int sock, s8** buf, u32* len);
SEND_SOCK* find_send_sock(int sock);
SEND_BUF* get_send_buf(int sock);

#endif

