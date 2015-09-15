
#include "type.h"
#include "buf.h"

#define HASH_SEND_SOCK_NUM 10

static SEND_SOCK g_send_sock[HASH_SEND_SOCK_NUM];

// init send sock

void init_send_sock(SEND_SOCK* p_send, int sock) {
	
	init_node(&p_sock-> node);
	init_node(&p_sock-> head);
	
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

static void delete_del_buf(SEND_BUF* p_buf) {
	
	delete_node(&p_buf-> node);
	
}

// get send buffer

STATUS get_send_buf(int sock, SEND_BUF** pp_send, s32* len) {
	
	SEND_SOCK* p_send;
	SEND_BUF* p_buf;
	ListNode* p_node;
	
	// find  send sock first
	
	p_send = find_send_sock(sock);
	if(NULL == p_send) {
		
		return FALSE;
	}
	
	// get node first
	
	p_node = p_send->head.next;
	if(p_node == &p_send->head) {
		
		return FALSE;
	}
	
	
	p_buf = (SEND_BUF*) p_node;
	** pp_send =  p_buf-> buf;
	*len = p_buf->len;
	delete_del_buf(p_buf);
	
	return TRUE;
}

