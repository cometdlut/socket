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
#define THREE_CARD     0x6
#define THREE_ONE      0x7
#define THREE_TW0      0x8
#define FOUR_TWO_DIFF  0x9
#define FOUR_TWO_SAME  0xa
#define MORE_DOUBLE    0xb
#define MORE_THREE     0xc
#define MORE_THREE_ONE 0xd
#define MORE_THREE_TWO 0xe
#define TYPE_ERR       0xffffffff

// mapping method

// 3 - 0, 4 - 1, 5 - 2, 6 - 3, 7 - 4, 8 - 5
// 9 - 6, 10 -7, j - 8, p - 9, k - 10, A - 11, 2 - 12

// check cheat function

static STATUS check_cheat(char card[], int length) {

	return FALSE;
}

// shuffle cards by real value

void resort_cards(char card[], int length) {

	int i;
	int j;
	char tmp;
	char var1;
	char var2;

	for(i = length -1; i >= 1; i --) {
		for(j = 0; j < i; j ++) {

			var1 = card[i];
			var2 = card[i + 1];

			if(var1 != 52 && var1 != 53)
				var1 = var1 % 13;

			if(var2 != 52 && var2 != 53)
				var2 = var2 % 13;

			if(var1 > var2) {

				tmp = card[i];
				card[i] = card[i +1];
				card[i + 1] = tmp;
			}
		}
	}
}

// check specified card

static STATUS is_same_card(char a, char b) {

	return ((a % 13) == (b % 13)) ? TRUE : FALSE;
}


static STATUS is_single_card(char card[], int length) {

	assert(card);
	assert(length == 1);

	return TRUE;
}


static STATUS is_double_queens(char card[], int length) {

	assert(card);
	assert(2 == length);

	if(card[0] == 52 && card[1] == 53) {

		return TRUE;
	}

	return FALSE;
}


static STATUS is_double_cards(char card[], int length) {

	assert(card);
	assert(2 == length);

	return is_same_card(card[0], card[1]);
}


static STATUS is_three_cards(char card[], int length) {

	assert(card);
	assert(3 == length);

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2])) {

		return TRUE;
	}

	return FALSE;
}


static STATUS is_bomb(char card[], int length) {

	assert(card);
	assert(4 == length);

	if(TRUE == is_same_card(card[0], card[1]) &&
		TRUE == is_same_card(card[1], card[2]) &&
		TRUE == is_same_card(card[2], card[3])) {

		return TRUE;
	}

	return FALSE;
}


static STATUS is_sequence(char card[], int length) {

	int i;

	assert(card);
	assert(length >= 5);

	for(i = 0; i < (length - 1); i ++) {

		if(TRUE != is_same_card(card[i] + 1, card[i]))
			return FALSE;
	}

	return TRUE;
}


static STATUS is_three_one(char card[], int length) {

	assert(card);
	assert(length == 4);

	if(TRUE == is_three_cards(card, 3) &&
		FALSE == is_same_card(card[2], card[3])) {

		return TRUE;

	}else if ( TRUE == is_three_cards(card + 1, 3) &&
		FALSE == is_same_card(card[0], card[1])) {

		return TRUE;
	}else {

		return FALSE;
	}
}

static STATUS is_three_two(char card[], int length) {

	assert(card);
	assert(length == 5);

	if(TRUE == is_three_cards(card, 3) &&
		TRUE == is_double_cards(card +3, 2) &&
		FALSE == is_same_card(card[2], card[3])) {

		return TRUE;

	}else if ( TRUE == is_three_cards(card + 2, 3) &&
		TRUE == is_double_cards(card, 2) &&
		FALSE == is_same_card(card[1], card[2])) {

		return TRUE;
	}else {

		return FALSE;
	}
}

static STATUS is_four_two_diff(char card[], int length) {

	int i;

	assert(card);
	assert(6 == length);

	for(i = 0; i < 3; i ++) {
		if(is_bomb(card + i, 4)) {
			break;
		}
	}

	if(3 == i) {
		return FALSE;
	}

	if(0 == i) {
		if(TRUE == is_same_card(card[4], card[5])) {
			return FALSE;
		}

		return TRUE;

	} if(1 == i) {
		if(TRUE == is_same_card(card[0], card[5])) {
			return FALSE;
		}

		return TRUE;
	}else {
		if(TRUE == is_same_card(card[0], card[1])) {
			return FALSE;
		}

		return  TRUE;
	}
}

static STATUS is_four_two_same(char card[], int length) {

	int i;

	assert(card);
	assert(8 == card);

	for(i = 0; i < 4; i ++) {
		if(TRUE == is_bomb(card +i, 4))
			break;
	}

	if((4 == i) || (i % 2))
		return FALSE;

	if(0 == i) {

		if(TRUE == is_double_cards(card +4, 2) &&
			TRUE == is_double_cards(card +6, 2) &&
			FALSE == is_same_card(card[5], card[6])) {

			return TRUE;
		}

		return FALSE;
	}else {

		if(TRUE == is_double_cards(card, 2) &&
			TRUE == is_double_cards(card +6, 2) &&
			FALSE == is_same_card(card[1], card[6])) {

			return TRUE;
		}

		return FALSE;
	}
}

static STATUS is_more_double(char card[], int length) {

	int i;

	assert(card);
	assert((length >= 6) && (0 == (length % 2)));

	// check same cards

	for(i = 0; i < (length >> 1); i ++) {

		if(FALSE == is_same_card(card[2*i], card[2*i] + 1)) {
			return FALSE;
		}
	}

	// check sequence

	for(i = 0; i < ((length >> 1) -1)) {

		if(FALSE == is_same_card(card[2*i] + 1, card[2*(i + 1)])) {
			return FALSE;
		}
	}

	return TRUE;
}

static STATUS is_more_three(char card[], int length) {

	int i;

	assert(card);
	assert((length >= 6) && (0 == (length % 3)));

	// check three cards

	for(i = 0; i < (length / 3); i ++) {
		if(FALSE == is_three_card(card + i * 3, 3))
			return FALSE;
	}

	// check sequence

	for(i = 0; i < (length / 3 - 1); i ++) {

		if(FALSE == is_same_card(card[3 *i] + 1, card[3*(i+1)]))
			return FALSE;
	}

	return TRUE;
}

static STATUS is_more_three_one(char card[], int length) {

	int i;
	int start;

	assert(card);
	assert((length >= 8) && (0 == length %4));

	// check if there is bomb

	for(i = 0; i < (length - 3); i ++) {

		if(TRUE == is_bomb(card + i, 4))
			return FALSE;
	}

	// check three cards

	for(i = 0; i < (length - 2); i ++) {

		if(TRUE == is_more_three(card + i, length - (length / 4)))
			break;
	}

	if(i == (length - 2))
		return FALSE;

	// check single card

	start = i;
	for(i = 0; i < length - 1; i ++) {

		if(i >= start && i < (start + length - (length /4)))
			continue;

		if((i+1) >= start && (i+1) < (start + length - (length /4)))
			continue;

		if(TRUE == is_same_card(card[i] + 1, card[i + 1]))
			return FALSE;
	}

	return TRUE;

}

static STATUS is_more_three_two(char card[], int length) {

	int i;
	int start;

	assert(card);
	assert((length >= 10) && (0 == length %5));

	// check if there is bomb

	for(i = 0; i < (length - 3); i ++) {

		if(TRUE == is_bomb(card + i, 4))
			return FALSE;
	}
	// check three card

	for(i = 0; i < (length - 2); i ++) {

		if(TRUE == is_more_three(card + i, length - (length / 5)))
			break;
	}

	if(i == (length - 2))
		return FALSE;

	// check single card

	start = i;
	i = 0;

	while(i < length - 1) {

		if(i >= start && i < (start + length - (length /5))) {

			i ++;
			continue;
		}

		if((i+1) >= start && (i+1) < (start + length - (length /5))) {

			i ++;
			continue;
		}

		if(FALSE == is_same_card(card[i], card[i] + 1)) {

			return FALSE;
		}

		i += 2;
	}

	return TRUE;
}

// check card type

static int  check_type(char* card, int length) {

	assert(length);
	
	switch(length) {
		
		case 1:
			/* check single card */
			
		case 2:

			/* check DOU_QUEENS, DOUBLE_CARD */
			
		case 3:

			/* check THREE_CARD */
			
		case 4:

			/* check BOMB, THREE_ONE */

		case 5:

			/* check SEQUENCE, THREE_TWO */

		case 6:

			/* check SEQUENCE, FOUR_TWO_DIFF, MORE_THREE, MORE_DOUBLE */

		case 7:

			/* check SEQUENCE */

		case 8:

			/* check SEQUENCE, FOUR_TWO_SAME, MORE_TWO, MORE_THREE_ONE */
			
		default:
		
			/* check SEQUENCE, MORE_DOUBLE, MORE_THREE, MORE_THREE_ONE, MORE_THREE_TWO */
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

		case THREE_CARD:

			if(type == THREE_CARD) {
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

		case THREE_TWO:

			if(type == THREE_TWO) {
				/* ... */
			}else if(type == DOU_QUEUES || type == BOMB) {
				/* ... */
			}else {
				assert(0);
			}

			break;

		case FOUR_TWO_DIFF:

			if(type == FOUR_TWO_DIFF) {
				/* ... */
			}else if(type == DOU_QUEUES || type == BOMB) {
				/* ... */
			}else {
				assert(0);
			}

			break;

		case FOUR_TWO_SAME:

			if(type == FOUR_TWO_SAME) {
				/* ... */
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

		case MORE_THREE:

			if(type == MORE_THREE){
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

		case MORE_THREE_TWO:

			if(type == MORE_THREE_TWO) {
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

