#include "deck.h"

#define SUITE_SIZE 4


void init_deck(Deck * deck){
	build_deck(deck);
	shuffle_deck(deck);
}

/* Knuth style shuffling */
void shuffle_deck(Deck * deck) {
	int i, index;
	for (i = 51; i >= 0; i--){
	    index = rand() % 51;
	    swap(&deck->card[i], &deck->card[index]);
	}
}

/* builds deck */
void build_deck(Deck * deck){
	int i, j, c;
	c = 0;
	for (i = 0; i < SUITE_SIZE; ++i){
		for (j = 0; j < VALUE_SIZE; ++j) {
			deck->card[c].suite = i;
			deck->card[c].value = j;
			c = c + 1;
		}
	}
	deck->head = 0;
	deck->tail = 0;
	deck->size = DECK_SIZE;
}

/* the way I'm implementing the queue mechanism is by tracking the header and tail 
indices on an array and enqueuing / dequeuing when needed */
Card deal_card(Deck * deck){ /* dequeue */
	Card return_card;
	Card temp;
	return_card.suite = 0;
	return_card.value = 0;
	if (deck->size == 0) return return_card; /* deck empty can't fill */

	temp = deck->card[deck->head];
	deck->card[deck->head].suite = -1; 
	deck->card[deck->head].value = -1;
	return_card = temp;
	deck->size--;
	deck->head++;
	if (deck->head == DECK_SIZE)
		deck->head = 0;
	return return_card;
}

void add_card(Deck * deck, Card card){ /* enqueue */
	if (deck->size == DECK_SIZE)
		return; /* can't add_card to a full deck */
	deck->card[deck->tail].suite = card.suite;
	deck->card[deck->tail].value = card.value;
	deck->tail = (deck->tail + 1) % DECK_SIZE;
	deck->size++;
	return;
}