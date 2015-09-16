
#include "type.h"
#include "buf.h"
#include "handle.h"

#define HASH_SEND_SOCK_NUM 10

static ListNode g_send_sock[HASH_SEND_SOCK_NUM];

// init send sock

void init_send_sock(SEND_SOCK* p_send, int sock) {
	
	init_node(&p_send-> node);
	init_node(&p_send-> head);
	
	p_send-> sock = sock;
	
}

// add send sock to header

void add_send_sock(SEND_SOCK* p_send) {
	
	int hash;
	
	hash = p_send-> sock % HASH_SEND_SOCK_NUM;
	
	add_node(&g_send_sock[hash], &p_send-> node);
}

// delete send sock

void delete_send_sock(SEND_SOCK* p_send) {
	
	delete_node(&p_send->node);
	
}

// find send sock

SEND_SOCK* find_send_sock(int sock) {
	
	SEND_SOCK* p_send;
	ListNode* p_node;
	int hash;
	
	hash = sock % HASH_SEND_SOCK_NUM;
	p_node = g_send_sock[hash].next;
	
	for(; p_node != &g_send_sock[hash]; p_node = p_node-> next) {
		
		p_send = (SEND_SOCK*)(p_node);
		if(p_send-> sock == sock) {
			
			return p_send;
		}
	}
	
	return 0x0;
}

// init send buf 

void init_send_buf(SEND_BUF* p_buf, s8* buf, u32 len) {
	
	init_node(&p_buf-> node);
	
	p_buf-> buf = buf;
	p_buf-> len = len;
	p_buf-> start = 0;
	
}

// add send buf

void add_send_buf(int sock, SEND_BUF* p_buf) {
	
	SEND_SOCK* p_send;
	
	// find send sock first
	
	p_send = find_send_sock(sock);
	if(NULL == p_send) {
		
		return;
	}
	
	// add send buf
	
	add_node(&p_send->head, &p_buf->node);
}

// delete send buffer

void delete_send_buf(SEND_BUF* p_buf) {
	
	delete_node(&p_buf-> node);
	
}

// get send buffer

SEND_BUF* get_send_buf(int sock) {
	
	SEND_SOCK* p_send;
	ListNode* p_node;
	
	// find  send sock first
	
	p_send = find_send_sock(sock);
	if(NULL == p_send) {
		
		return (SEND_BUF*)0x0;
	}
	
	// get node first
	
	p_node = p_send->head.next;
	if(p_node == &p_send->head) {
		
		return (SEND_BUF*)0x0;
	}
	
	return (SEND_BUF*) p_node;
}

// init rcv buffer

void init_rcv_buf(RCV_BUF* p_rcv, s8* buf, u32 len) {

	init_node(&p_rcv->node);

	p_rcv->buf = buf;
	p_rcv->len = len;

}

// add buf to socket

STATUS add_buf_to_sock(int sock, RCV_BUF* p_rcv) {

	SOCK_HANDLE* p_hand;

	p_hand = find_handle(sock);
	if(!p_hand){

		return FALSE;
	}

	add_node(&p_hand->read, &p_rcv-> node);

}

// get buf from socket

STATUS get_rcv_buf(int sock, s8** buf, u32* len) {

	SOCK_HANDLE* p_hand;
	RCV_BUF* p_rcv;
	ListNode* p_node;

	p_hand = find_handle(sock);
	if(!p_hand) {

		return FALSE;
	}

	p_node = p_hand->read.next;
	if(p_node == &p_hand->read) {

		return FALSE;
	}

	p_rcv = (RCV_BUF*)p_node;
	*buf = p_rcv-> buf;
	*len = p_rcv->len;

	delete_node(p_node);

	return TRUE;

}



