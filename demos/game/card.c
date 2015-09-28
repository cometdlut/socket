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
#include <stdlib.h>
#include <assert.h>

// define struct card

typedef struct _CARD{
	
	LinkNode node;
	int num;
}CARD;

// current holding card num

static int current_card_num;

// current game state

static int current_game_state;

// current card owner

static int current_owner;

// current card type

static int current_card_type;

// current card data

static char local_card[54 /3 + 3];

// current boss

static int current_boss;

// current bet

static int current_bet;

// define game state

#define INIT_STATE   0x0
#define SELECT_STATE 0x1
#define GAME_STATE   0x2
#define RUN_STATE    0x3

// select boss

void select_boss() {


}

// deal card first

void deal_card() {
	
}

// check card type

#define SINGLE_CARD    0x1
#define DOUBLE_CARD    0x2
#define BOMB           0x3
#define SEQUENCE       0x4
#define DOU_QUEUES     0x5
#define THREE_ONE      0x6
#define MORE_DOUBLE    0x7
#define MORE_THREE_ONE 0x8

static int  check_type(char* card, int length) {

	assert(length);
	
	switch(length) {
		
		case 1:
			return SINGLE_CARD;
			
		case 2:
			if(card[0] == SMALL_QUEEN && card[1] == BIG_QUEEN) {
				return DOU_QUEUES;
				
			} else if((card[0] % 13) == (card[1] % 13)) {
				return DOUBLE_CARD;
			} else 
				assert(0);
			
		case 3:
			assert(0);
			
		case 4:
			/* check BOMB, MORE_DOUBLE, THREE_ONE */
			
		default:
		
			if(length % 2) {
			
				/* check SEQUENCE */
			}else {
				/* check SEQUENCE, MORE_DOUBLE, MORE_THREE_ONE */
			}
	}
}

static _process_card(char* card, int length)
{
	int type;
	
	type = check_type(card, length);
	
	switch(current_card_type) {
		
		case SINGLE_CARD:
		
			if(type == SINGLE_CARD) {
				/*  */
			}else if(type == DOU_QUEUES || type == BOMB) {
				/*  */
			}else {
				assert(0);
			}
			break;
			
		case DOUBLE_CARD:
		
			if(type == DOUBLE_CARD) {
				/* ... */
			}else if(type == DOU_QUEUES || type == BOMB) {
				/* ... */
			}else {
				assert(0);
			}
			break;
			
		case THREE_ONE:
		
			if(type == THREE_ONE) {
				/* .... */
			}else if(type == DOU_QUEUES || type == BOMB) {
				/* ... */
			}else {
				assert(0);
			}
			break;
			
		case SEQUENCE:
		
			if(type == SEQUENCE) {
				/* ...*/
			}else if(type == DOU_QUEUES || type == BOMB) {
				/* ... */
			}else {
				assert(0);
			}
			
			break;
			
		case BOMB:
		
			if(type == BOMB) {
				/* ... */
			}else if(type == DOU_QUEUES) {
				/* ... */
			}else {
				assert(0);
			}
			
			break;
			
		case MORE_DOUBLE:
		
			if(type == MORE_DOUBLE){
				/* ... */
			}else if(type == DOU_QUEUES || type == BOMB) {
				/* ... */
			}else {
				assert(0);
			}
			
			break;
		
		case MORE_THREE_ONE:
		
			if(type == MORE_THREE_ONE) {
				/* ... */
			}else if(type == DOU_QUEUES || type == BOMB) {
				/* ... */
			}else {
				assert(0);
			}

			break;

		default:
			assert(0);
	}
}

// process card

void process_card(char* card[], int length, int sender, int finish) {
	
	// check if finish already
	
	if(finish) {
		
		return;
	}
	
	// someone try to give up this opportunity
	
	if(0 == length) {
		if(GAME_STATE == current_game_state) {
			assert(0);
		}
		
		return;
	}
	
	// first get card from user
	
	if(0 == current_owner) {
		
		current_card_num = length;
		current_owner = sender;
		memmove(local_card, card, length);
		
		current_game_state = RUN_STATE;
		current_card_type = check_type();
		
		return;
	}
	
	// sender equals to owner
	
	if(sender == owner) {
		
		current_card_num = length;
		memmove(local_card, card, length);
		current_card_type = check_type();
		
		return;
	}
	
	// other ways of processing
	
	_process_card(card, length);
	
	memmove(local_card, card, length);
	current_card_num = length;
	current_owner = sender;
	current_card_type = check_type();
	
	return;
}



// entry of all function

int main(int argc, char* argv[]) {

	// init module
	
	// get event
	
	while(1) {
		
		// analyse card
		
		// make logic assessment
		
		// broadcast information to all users
		
		// update score if necessary
		
	}

	return 0;
}
