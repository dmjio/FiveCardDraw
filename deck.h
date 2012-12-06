#ifndef __DECK_H_
#define __DECK_H_

#include <locale.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "card.h"

typedef struct {
	Card card[DECK_SIZE];
	void (*add_card) (Card *);
	Card (*deal_card) (void);
	int size;
	int head;
	int tail;
} Deck;

void build_deck();
void shuffle_deck(Deck *);
Card deal_card(Deck * ); /* dequeue */
void add_card(Deck * , Card ); /* enqueue */
void init_deck(Deck * );

#endif