#ifndef __POKER_H_

#define __POKER_H_

#include <stdio.h>
#include <stdlib.h>
#define DECK_SIZE 52
#define SUITE_SIZE 4
#define PLAYER_SIZE 4
#define VALUE_SIZE 13
#define HAND_SIZE 5

enum VALUE { Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
enum SUITE { Hearts = 0, Spades, Diamonds, Clubs };

typedef struct Card_ {
	int value;
	int suite;
	char * (*to_suite)(int);
	char * (*to_val)(int);
} Card;

typedef struct Exchange_  {
	Card card[5];
	int count;
} Exchange;

typedef struct Hand_ {
	Card card[HAND_SIZE];
	int rank;
} Hand;

typedef struct Deck_ {
	Card card[DECK_SIZE];
	void (*add_card) (Card *);
	Card (*deal_card) (void);
	int size;
	int head;
	int tail;
} Deck;

typedef struct Player_ {
	Hand * hand;
	char * name;
	int bet;
	int isComputer;
} Player;

/* deck abstractions */
void build_deck();
void print(Card * deck, int size);
void shuffle_deck(Deck *);
Card deal_card(Deck * deck); /* dequeue */
void add_card(Deck * deck, Card card); /* enqueue */
Exchange exchange(Deck *, Player *, int);

/* card helpers */
wchar_t * to_suite(int suite);
char * to_val(int value);
int card_comparator(const void * a, const void * b);
void sort_cards(Card * cards, int size);

/* player abstractions */
void build_players(Player *);
void deal_players(Player *, Deck *);
void prompt_player(Player * p, Deck * d);

/* init players */
void init_players(Player * players, int);
char * to_player(int i);
void print_hand(Player * p);
void print(Card * deck, int);

/* hand helpers */
void best_hand(Card * hand);
void swap(Card*, Card*);

/* insert into exchange */
void insert_exchange_into_hand(Exchange*, Hand*, int);

#endif



