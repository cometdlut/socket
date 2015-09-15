
#include "type.h"
#include "timer.h"
#include "log.h"

static int g_tick;
static ListNode g_timer_head;

// init timer module

void timer_init() {

	g_tick = 0;
	init_node(&g_timer_head);
}

// register timer module

void timer_add(int gap, void(*func)(void*), void* param) {

	timer tm;
	timer* p_tm;
	ListNode* p_node;

	init_node(&tm.node);
	tm.tick = g_tick + gap;
	tm.func = func;
	tm.param = param;

	p_node = g_timer_head.next;
	for(; p_node != &g_timer_head; p_node = p_node->next) {

		p_tm = (timer*) (p_node);
		if(p_tm-> tick > tm.tick){
			break;
		}
	}

	add_node(p_node, &tm.node);
}

// delete timer module

void timer_delete(timer* p_tm) {

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

		p_tm->func(p_tm-> param);

	}

}

