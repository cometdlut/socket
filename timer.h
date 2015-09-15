
#ifndef _TIMER_H
#define _TIMER_H

#include "list.h"

// define struct

typedef struct _timer {

	ListNode node;
	u32 tick;
	void(*func)(void*);
	void* param;

}timer;

// define function


void timer_init();
void timer_add(int gap, void(*func)(void*), void* param);
void timer_del(timer* p_tm);
void update_timer();

#endif
