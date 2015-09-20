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
 
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#include "type.h"
#include "timer.h"
#include "log.h"

static int g_tick;
static ListNode g_timer_head;
static struct itimerval oldtv;

// init timer module

void init_timer() {

	struct itimerval itv;

	// init global

	g_tick = 0;
	init_node(&g_timer_head);

	// init timer

	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itv, &oldtv);
}

// init timer struct

static void init_timer_struct(Timer* p_tm, int gap, void(*func)(void*), void* param) {

	init_node(&p_tm->node);

	p_tm-> tick = 0;
	p_tm-> func = func;
	p_tm-> param = param;
	p_tm-> gap = gap;
}

// create timer function

Timer* create_timer(int gap, void(*func)(void*), void* param) {

	Timer* p_tm;

	// if timer == 0 or param not valid,
	// just return from add_timer function

	if(!gap || !func || !param) {

		return 0x0;
	}

	p_tm = (Timer*) malloc(sizeof(Timer));
	if(!p_tm) {

		return 0x0;
	}

	init_timer_struct(p_tm, gap, func, param);
	return p_tm;
}

// register timer module

void add_timer(Timer* p_cur) {

	ListNode* p_node;
	Timer* p_tm;

	p_cur-> tick = g_tick + p_cur-> gap;

	p_node = g_timer_head.next;
	for(; p_node != &g_timer_head; p_node = p_node->next) {

		p_tm = (Timer*) (p_node);
		if(p_tm-> tick > p_cur-> tick){
			break;
		}
	}

	add_node(p_node, &p_cur->node);
}

// delete timer module

void delete_timer(Timer* p_tm) {

	delete_node(&p_tm->node);
}

// update timer by period function

void update_timer() {

	Timer* p_tm;
	ListNode* p_prev;
	ListNode* p_node;

	g_tick ++;
	p_node = g_timer_head.next;

	while(1){

		if(p_node == &g_timer_head){
			break;
		}

		p_tm = (Timer*)(p_node);
		if(p_tm->tick > g_tick) {
			break;
		}

		p_prev = p_node;
		p_node = p_node-> next;

		delete_node(p_prev);
		p_tm->func(p_tm-> param);

	}

}

// remove timer

void remove_all_timer() {

	ListNode* p_node;
	ListNode* p_prev;
	Timer* p_tm;

	p_node = g_timer_head.next;

	while(p_node != &g_timer_head) {

		p_tm = (Timer*) p_node;
		p_prev = p_node;
		p_node = p_node-> next;

		delete_node(p_prev);
		free(p_tm);
	}

}

