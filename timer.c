
#include "type.h"
#include "timer.h"
#include "log.h"

static int g_tick;
static ListNode g_timer_head;

// init timer module

void init_timer() {

	g_tick = 0;
	init_node(&g_timer_head);
}

// init timer struct

void init_timer_struct(timer* p_tm, int gap, void(*func)(void*), void* param) {


	// if timer == 0 or param not valid,
	// just return from add_timer function

	if(!p_tm, !gap || !func || param) {

		return;
	}

	init_node(&p_tm->node);
	p_tm-> tick = g_tick + gap;
	p_tm-> func = func;
	p_tm-> param = param;
}



// register timer module

void add_timer(timer* p_cur) {

	ListNode* p_node;
	timer* p_tm;

	p_node = g_timer_head.next;
	for(; p_node != &g_timer_head; p_node = p_node->next) {

		p_tm = (timer*) (p_node);
		if(p_tm-> tick > p_cur-> tick){
			break;
		}
	}

	add_node(p_node, &p_cur->node);
}

// delete timer module

void delete_timer(timer* p_tm) {

	delete_node(&p_tm->node);
}

// update timer by period function

void update_timer() {

	timer* p_tm;
	ListNode* p_prev;
	ListNode* p_node;

	g_tick ++;
	p_node = g_timer_head.next;

	while(1){

		if(p_node == &g_timer_head){
			break;
		}

		p_tm = (timer*)(p_node);
		if(p_tm->tick > g_tick) {
			break;
		}

		p_prev = p_node;
		p_node = p_node-> next;

		delete_node(p_prev);
		p_tm->func(p_tm-> param);

	}

}

