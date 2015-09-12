
#include "type.h"
#include "list.h"

void init_node (ListNode* node) {

	node->prev = node;
	node->next = node;

}

void add_node(ListNode* head, ListNode* node){

	node->next = head;
	node->prev = head->prev;
	head->prev->next = node;
	head->prev = node;
}

void delete_node(ListNode* node){

	node->prev->next = node-> next;
	node->next->prev = node-> prev;

}

STATUS is_list_empty(ListNode* head) {

	if(head == head-> next)
		return TRUE;

	return FALSE;

}


