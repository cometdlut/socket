
#include <malloc.h>

#include "type.h"
#include "handle.h"
#include "assert.h"
#include "buf.h"
#include "define.h"

// define global variable

static ListNode g_handle[HANDLE_HASH_NUM];

static func g_onConnect;
static func g_onClose;
static func g_onRecv;


// define function here

void init_handle() {

	int i;

	// init node

	for(i = 0; i < HANDLE_HASH_NUM; i ++) {

		init_node(&g_handle[i]);
	}

	// init func parameters

	g_onConnect = 0x0;
	g_onClose = 0x0;
	g_onRecv = 0x0;
}

// init struct

static void init_handle_struct(SOCK_HANDLE* p_hand, int sock) {

	init_node(&p_hand->node);
	init_node(&p_hand->read);

	p_hand-> sock = sock;
	p_hand-> onConnect = g_onConnect;
	p_hand-> onClose = g_onClose;
	p_hand-> onRecv = g_onRecv;
}

// register sock

static void register_sock(SOCK_HANDLE* p_hand) {

	add_node(&g_handle[p_hand-> sock % HANDLE_HASH_NUM], &p_hand->node);
}

void register_callback_func(func onConnect, func onClose, func onRecv) {

	g_onConnect = onConnect;
	g_onClose = onClose;
	g_onRecv = onRecv;
}

// remove sock

static void remove_sock(SOCK_HANDLE* p_hand) {

	delete_node(&p_hand->node);
}

// find sock

SOCK_HANDLE* find_handle(int sock) {

	SOCK_HANDLE* p_hand;
	ListNode* p_node;
	int num;

	num = sock % HANDLE_HASH_NUM;
	p_node = g_handle[num].next;

	for(; p_node != &g_handle[num]; p_node = p_node-> next){

		p_hand = (SOCK_HANDLE*)(p_node);
		if(p_hand-> sock == sock) {

			return p_hand;
		}
	}

	return (SOCK_HANDLE*)0x0;
}

// process sock message from epoll

void process_message(int type, int sock) {

	SOCK_HANDLE* p_hand;
	s8* p_buf;
	u32 len;

	switch(type) {

		case NEW_SOCK:

			p_hand = (SOCK_HANDLE*) malloc(sizeof(SOCK_HANDLE));
			if(!p_hand) {
				assert(0);
			}

			// init struct and register sock to whole structure

			init_handle_struct(p_hand, sock);
			register_sock(p_hand);

			p_hand->onConnect(p_hand);

			break;

		case SOCK_CLOSE:

			// remove sock when it is not connected already

			p_hand = find_handle(sock);
			if(p_hand) {

				p_hand->onClose(p_hand);
			}


			// remove all rcv buffer data

			while(TRUE == get_rcv_buf(p_hand-> sock, &p_buf, &len)) {

				free(p_buf);
			}

			remove_sock(p_hand);
			free(p_hand);

			break;

		case RECV_DATA:

			p_hand = find_handle(sock);
			if(p_hand) {

				p_hand->onRecv(p_hand);
			}

			break;


		default:
			assert(0);
	}

}

// send buf data

STATUS send_buf(SOCK_HANDLE* p_hand, s8* buf, u32 len) {

	SEND_BUF* p_buf;

	// add send buf

	p_buf = (SEND_BUF*) malloc(sizeof(SEND_BUF));
	if(!buf) {

		assert(0);
		return FALSE;
	}

	init_send_buf(p_buf, buf, len);
	add_send_buf(p_hand-> sock, p_buf);

	// modify event

	epoll_mod_socket(p_hand-> sock);

	return TRUE;

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
	free(p_rcv);

	return TRUE;

}

// dump rcv sock

void dump_all_recv_sock() {

	SOCK_HANDLE* p_hand;
	ListNode* p_node;
	s32 i;

	log_print_msg("All rcv sock is as follows:\n");

	for(i = 0; i < HANDLE_HASH_NUM; i ++) {

		p_node = g_handle[i].next;
		if(p_node != &g_handle[i]) {

			continue;
		}

		p_hand = (SOCK_HANDLE*) p_node;
		log_print_msg("    %d\n", p_hand->sock);

	}

}

// dump rcv buf by sock

void dump_rcv_buf(int sock) {

	SOCK_HANDLE* p_hand;
	RCV_BUF* p_rcv;
	ListNode* p_node;

	log_print_msg("Rcv buf is as follows:\n");

	p_hand = find_handle(sock);
	if(!p_hand) {

		log_print_msg("    No sock.\n");
		return;
	}

	p_node = p_hand->read.next;
	if(p_node == &p_hand->read) {

		log_print_msg("    No rcv buf.\n");
		return;
	}

	for(; p_node != &p_hand->read; p_node = p_node-> next) {

		p_rcv = (RCV_BUF*)(p_node);

		log_print_msg("    addr:0x%x\n", p_rcv-> buf);
		log_print_msg("    len:%d\n", p_rcv-> len);
		log_print_msg("\n");
	}
}


