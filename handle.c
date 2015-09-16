
#include <malloc.h>

#include "type.h"
#include "handle.h"
#include "assert.h"
#include "buf.h"

// define global variable

#define HANDLE_HASH_NUM 10

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

			remove_sock(p_hand);

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

