#ifndef __EXCHANGE_H_
#define __EXCHANGE_H_

#include "deck.h"
#include "player.h"
#include "hand.h"

typedef struct {
	Card card[5];
	int count;
} Exchange;

Exchange * exchange_card(Deck * , Exchange * , int );
void insert_exchange_into_hand(Exchange*e, Hand*h, int size);


#endif
