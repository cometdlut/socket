
#ifndef _LIST_H
#define _LIST_H

// struct definition

struct _ListNode {
	
	struct _ListNode* prev;
	struct _ListNode* next;
};

typedef struct _ListNode ListNode;

// type definition

void init_node(ListNode*);
void add_node(ListNode*, ListNode*);
void delete_node(ListNode*);
STATUS is_list_empty(ListNode*);


#endif

