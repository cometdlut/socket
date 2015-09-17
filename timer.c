
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "type.h"
#include "timer.h"
#include "log.h"

static int g_tick;
static ListNode g_timer_head;
static struct itimerval oldtv;

// init timer module

void init_timer() {

	// init global

	g_tick = 0;
	init_node(&g_timer_head);

	// init timer

	struct itimerval itv;
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itv, &oldtv);
}

// init timer struct

void init_timer_struct(Timer* p_tm, int gap, void(*func)(void*), void* param) {

	init_node(&p_tm->node);
	p_tm-> tick = g_tick + gap;
	p_tm-> func = func;
	p_tm-> param = param;
}

// create timer function

STATUS create_timer(int gap, void(*func)(void*), void* param) {

	Timer* p_tm;

	// if timer == 0 or param not valid,
	// just return from add_timer function

	if(!gap || !func || !param) {

		return FALSE;
	}

	p_tm = (Timer*) malloc(sizeof(Timer));
	if(!p_tm) {

		return FALSE;
	}

	init_timer_struct(p_tm, gap, func, param);
	return TRUE;
}

// register timer module

void add_timer(Timer* p_cur) {

	ListNode* p_node;
	Timer* p_tm;

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

