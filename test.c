#include "test.h"
#define TRUE 1
#define FALSE 0

int test_exchange(){
	Deck * deck = malloc(sizeof(Deck));
	Hand * hand = malloc(sizeof(Hand));
	Exchange * ex = malloc(sizeof(Exchange));

	build_deck(deck);
	shuffle_deck(deck);


	return 0;
}

int test_queue(){
	int i,j;
	Card card;
	Deck * deck = malloc(sizeof(Deck));
	Deck * deck2 = malloc(sizeof(Deck));

	Hand * hand = malloc(sizeof(Hand));

	build_deck(deck);
	shuffle_deck(deck);
	build_deck(deck2);
	shuffle_deck(deck2);
	i = 0;
	for (;;){

	printf("Initialized Deck and Hand\n");
	print(deck->card, DECK_SIZE);
	print(hand->card, HAND_SIZE);

	for (j = 0; j < HAND_SIZE; ++j){
		card = deal_card(deck);
		hand->card[j] = card;
	}

	printf("\nAdded cards to hand\n");

	print(deck->card, DECK_SIZE);
	print(hand->card, HAND_SIZE);

	for (j = 0; j < HAND_SIZE; ++j){
		add_card(deck, hand->card[j]);
		hand->card[j].suite = -1;
		hand->card[j].value = -1;
	}

	printf("\nPut cards back into deck\n");

	print(deck->card, DECK_SIZE);
	print(hand->card, HAND_SIZE);

	printf("\n\n");
	i++;
		if (i == 100)
			break;
	}


	sort_cards(deck->card, DECK_SIZE);
	sort_cards(deck2->card, DECK_SIZE);	

	for (i = 0; i < DECK_SIZE; ++i)
		if (deck->card[i].value != deck2->card[i].value) printf("Bad Shuffle\n");

	printf("Good Shuffle\n");

	return 0;

}

int exp_value(){
	/* Two-Pair */

	Hand * hand = malloc(sizeof(Hand) * HAND_SIZE);

	hand->card[0].suite=Spades; 	hand->card[0].value = Ace;
	hand->card[1].suite=Diamonds; hand->card[1].value = Ace;
	hand->card[2].suite=Spades; 	hand->card[2].value = Seven;
	hand->card[3].suite=Clubs; 	hand->card[3].value = Three;
	hand->card[4].suite=Diamonds; hand->card[4].value = Three;

	compute_expected_value(hand, TRUE);

	return 0;

}

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

int test_hands(){
	int i;
	Hand * hand = malloc(sizeof(Hand) * HAND_SIZE);

	for (i = 0; i < HAND_SIZE; ++i) {
		hand->card[i].suite=Hearts;
		hand->card[i].value = Ten + i;
	}
	
	best_hand(hand, TRUE, TRUE); /* should print royal flush for Hearts */
	printf("grade: %d\n", hand->value);
	/* Straight flush */

	for (i = 0; i < HAND_SIZE; ++i) {
		hand->card[i].suite=Hearts;
		hand->card[i].value = Two + i;
	}
	best_hand(hand, TRUE, TRUE); /* should print straight flush for Spades */
	printf("grade: %d\n", hand->value);

	/* four of a kind */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		hand->card[i].suite=Hearts+i;
		hand->card[i].value = Ace;
	}
	hand->card[4].suite = Diamonds;
	hand->card[4].value = Three;
	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	/* full house */
	for (i = 0; i < 3; ++i) {
		hand->card[i].suite=Hearts+i;
		hand->card[i].value = King;
	}
	for (i = 0; i < 2; ++i) {
		hand->card[i+3].suite=Hearts+i;
		hand->card[i+3].value = Ace;
	}
	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	/* flush */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		hand->card[i].suite=Hearts;
		hand->card[i].value = Five+i;
	}
	hand->card[4].suite=Hearts;
	hand->card[4].value = Ace;
	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	/* straight */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		hand->card[i].suite=Hearts;
		hand->card[i].value = Five+i;
	}	
	hand->card[4].suite=Clubs;
	hand->card[4].value = Four;
	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	/* three-of-a-kind */
	for (i = 0; i < HAND_SIZE-2; ++i) {
		hand->card[i].suite=Hearts+i;
		hand->card[i].value = Ace;
	}	
	hand->card[3].suite=Hearts; hand->card[3].value = Three;
	hand->card[4].suite=Diamonds; hand->card[4].value = Five;

	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	/* Two-Pair */
	hand->card[0].suite=Spades; 	hand->card[0].value = Ace;
	hand->card[1].suite=Diamonds; hand->card[1].value = Ace;
	hand->card[2].suite=Spades; 	hand->card[2].value = Seven;
	hand->card[3].suite=Clubs; 	hand->card[3].value = Three;
	hand->card[4].suite=Diamonds; hand->card[4].value = Three;
	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	/* One-Pair */
	hand->card[0].suite=Spades; 	hand->card[0].value = Ace;
	hand->card[1].suite=Diamonds; hand->card[1].value = Ace;
	hand->card[2].suite=Clubs; 	hand->card[2].value = Three;
	hand->card[3].suite=Diamonds; hand->card[3].value = Four;
	hand->card[4].suite=Spades; 	hand->card[4].value = Seven;
	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	/* High Card */
	hand->card[0].suite=Spades; 	hand->card[0].value = Ace;
	hand->card[1].suite=Diamonds; hand->card[1].value = King;
	hand->card[2].suite=Clubs; 	hand->card[2].value = Ten;
	hand->card[3].suite=Diamonds; hand->card[3].value = Nine;
	hand->card[4].suite=Spades; 	hand->card[4].value = Seven;
	best_hand(hand, TRUE, TRUE); 
	printf("grade: %d\n", hand->value);

	check_good_shuffle();

	return 0;
}