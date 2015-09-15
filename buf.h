
#ifndef _BUF_H
#define _BUF_H

#include "list.h"

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

#endif

