#include "exchange.h"

/* should be self-explanatory, I'm putting the values of the exchange struct from the deck back into the hand */
void insert_exchange_into_hand(Exchange*e, Hand*h, int size){
	int i;
	Card temp;
	i = 0;
	sort_cards(h->card, HAND_SIZE);
	for (i=0;i<size;i++){
			temp = e->card[i];
			e->card[i] = h->card[i];
			h->card[i] = temp;
		}
	}

/* exchanging cards from the deck into the exchange struct */
Exchange * exchange_card(Deck * deck, Exchange * e, int count){
	Card card;
	int i;
	for (i = 0; i < count; ++i){
		card = deal_card(deck);
		add_card(deck, e->card[i]);
		e->card[i] = card;
	}
	return e;
}
	