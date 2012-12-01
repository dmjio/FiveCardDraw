#include "test.h"


void check_good_shuffle(){
	int i;
	Deck * deck1 = malloc(sizeof(Deck));
	Deck * deck2 = malloc(sizeof(Deck));

	build_deck(deck1);
	build_deck(deck2);

	shuffle_deck(deck2);

	sort_cards(deck1->card, DECK_SIZE);
	sort_cards(deck2->card, DECK_SIZE);

	print(deck1->card, DECK_SIZE);
	print(deck2->card, DECK_SIZE);
	
	for (i = 0; i < DECK_SIZE; ++i)
		if (deck1->card[i].value != deck2->card[i].value)
		{
			printf("Bad Shuffle\n");
			return;
		}
	printf("Good Shuffle\n");
	return;

}

void test_hands(){
	int i;
	Card * hand = malloc(sizeof(Card) * HAND_SIZE);

	for (i = 0; i < HAND_SIZE; ++i)
	{
		hand[i].suite=Hearts;
		hand[i].value = Ten + i;
	}
	print(hand, HAND_SIZE);
	best_hand(hand); /* should print royal flush for Hearts */

	/* Straight flush */

	for (i = 0; i < HAND_SIZE; ++i) {
		hand[i].suite=Hearts;
		hand[i].value = Two + i;
	}
	print(hand, HAND_SIZE);
	best_hand(hand); /* should print straight flush for Spades */

	/* four of a kind */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		hand[i].suite=Hearts+i;
		hand[i].value = Ace;
	}
	hand[4].suite = Diamonds;
	hand[4].value = Three;
	print(hand, HAND_SIZE);
	best_hand(hand); 

	/* full house */
	for (i = 0; i < 3; ++i) {
		hand[i].suite=Hearts+i;
		hand[i].value = King;
	}
	for (i = 0; i < 2; ++i) {
		hand[i+3].suite=Hearts+i;
		hand[i+3].value = Ace;
	}
	print(hand, HAND_SIZE);
	best_hand(hand); 

	/* flush */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		hand[i].suite=Hearts;
		hand[i].value = Five+i;
	}
	hand[4].suite=Hearts;
	hand[4].value = Ace;
	print(hand, HAND_SIZE);
	best_hand(hand); 

	/* straight */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		hand[i].suite=Hearts;
		hand[i].value = Five+i;
	}	
	hand[4].suite=Clubs;
	hand[4].value = Four;
	print(hand, HAND_SIZE);
	best_hand(hand); 

	/* three-of-a-kind */
	for (i = 0; i < HAND_SIZE-2; ++i) {
		hand[i].suite=Hearts+i;
		hand[i].value = Five;
	}	
	hand[3].suite=Diamonds; hand[3].value = Seven;
	hand[4].suite=Clubs; hand[4].value = Ace;

	print(hand, HAND_SIZE);
	best_hand(hand); 

	/* Two-Pair */
	hand[0].suite=Spades; 	hand[0].value = Ace;
	hand[1].suite=Diamonds; hand[1].value = Ace;
	hand[2].suite=Spades; 	hand[2].value = Seven;
	hand[3].suite=Clubs; 	hand[3].value = Three;
	hand[4].suite=Diamonds; hand[4].value = Three;
	print(hand, HAND_SIZE);
	best_hand(hand); 

	/* One-Pair */
	hand[0].suite=Spades; 	hand[0].value = Ace;
	hand[1].suite=Diamonds; hand[1].value = Ace;
	hand[2].suite=Clubs; 	hand[2].value = Three;
	hand[3].suite=Diamonds; hand[3].value = Four;
	hand[4].suite=Spades; 	hand[4].value = Seven;
	print(hand, HAND_SIZE);
	best_hand(hand); 

	/* High Card */
	hand[0].suite=Spades; 	hand[0].value = Ace;
	hand[1].suite=Diamonds; hand[1].value = King;
	hand[2].suite=Clubs; 	hand[2].value = Ten;
	hand[3].suite=Diamonds; hand[3].value = Nine;
	hand[4].suite=Spades; 	hand[4].value = Seven;

	print(hand, HAND_SIZE);
	best_hand(hand); 


}