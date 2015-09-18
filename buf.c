/*
 * Copyright 2015 tristan_fei <feixiaoxing@163.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "type.h"
#include "buf.h"
#include "handle.h"
#include "log.h"
#include "define.h"

static ListNode g_send_sock[HASH_SEND_SOCK_NUM];

// init buffer

void init_buffer() {

	int i;

	for(i = 0; i < HASH_SEND_SOCK_NUM; i ++) {

		init_node(&g_send_sock[i]);
	}
}

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

// dump all send sock

void dump_all_send_sock() {

	s32 i;
	ListNode* p_node;
	SEND_SOCK* p_send;

	log_print_msg("Sock is as follows:\n");

	for(i = 0; i < HASH_SEND_SOCK_NUM; i ++) {

		p_node = g_send_sock[i].next;
		if(p_node == &g_send_sock[i]) {

			continue;
		}

		for(; p_node != &g_send_sock[i]; p_node = p_node-> next) {

			p_send = (SEND_SOCK*)(p_node);
			log_print_msg("    %d\n", p_send-> sock);
		}
	}
}

// dump send buf by sock

void dump_send_buf(int sock) {

	SEND_SOCK* p_sock;
	SEND_BUF* p_buf;
	ListNode* p_node;

	p_sock = find_send_sock(sock);
	if(!p_sock) {

		return;
	}

	p_node = p_sock->head.next;
	if(p_node == &p_sock-> head) {

		return;
	}

	log_print_msg("Send buf is as follows:\n");
	for(; p_node != &p_sock-> head; p_node = p_node-> next) {

		p_buf = (SEND_BUF*)(p_node);

		log_print_msg("    addr: 0x%x\n", p_buf->buf);
		log_print_msg("    len: %d\n", p_buf->len);
		log_print_msg("    start: %d\n", p_buf->start);
	}
}


