#ifndef __POKER_H_

#define __POKER_H_

#include <stdio.h>
#include <stdlib.h>
#define DECK_SIZE 52
#define SUITE_SIZE 4
#define PLAYER_SIZE 4
#define VALUE_SIZE 13
#define HAND_SIZE 5
#define TOTAL_CASH 400
#define ANTE_AMOUNT 5

enum VALUE { Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
enum SUITE { Hearts = 0, Spades, Diamonds, Clubs };

typedef struct {
	int value;
	int suite;
	char * (*to_suite)(int);
	char * (*to_val)(int);
} Card;

typedef struct {
	Card card[5];
	int count;
} Exchange;

typedef struct {
	Card card[HAND_SIZE];
	Card highCard;
	int rank;
	int _class;
	int value;
	void (*calc_hand_value)(int _class, int rank);
} Hand;

typedef struct {
	Card card[DECK_SIZE];
	void (*add_card) (Card *);
	Card (*deal_card) (void);
	int size;
	int head;
	int tail;
} Deck;

typedef struct {
	Hand * hand;
	int isBetter;
	int hasFolded;
	char * name;
	int isActive;
	int bet;
	int isComputer;
	int amount;
} Player;

typedef struct {
	int value;
	int player_count;
	Deck * deck;
	Player * players;
	int pot;
	int ante;
} Game;

/* game abstractions */
int start_game();
void init_game(Game * game);
int game_winner(Game *, int);
void reset_game(Game * g);

/* ante helpers */
void ante_up(Game *);

/* deck abstractions */
void build_deck();
void print(Card * , int );
void shuffle_deck(Deck *);
Card deal_card(Deck * ); /* dequeue */
void add_card(Deck * , Card ); /* enqueue */
Exchange * exchange_card(Deck * , Exchange * , int );
void init_deck(Deck * );

/* card helpers */
wchar_t * to_suite(int );
char * to_val(int );
int card_comparator(const void * , const void * );
void sort_cards(Card * , int );
char * to_card(Card );
void print(Card * , int);

/* player abstractions */
void build_players(Player *);
void deal_players(Player *, Deck *);
void prompt_player(Player * , Deck * );
void show_hands(Player * players, int player_count, int lastRound);

/* players */
void init_players(Player * , int);
char * to_player(int );
void print_hand(Player * );
void determine_winner(Game * , Player * , int );

/* betting rounds */
void betting_round(Game * , Player * , int );

/* hand helpers */
void best_hand(Hand * hand, int isFinalRound, int isHuman);
void swap(Card*, Card*);

/* insert into exchange */
void insert_exchange_into_hand(Exchange*, Hand*, int);

/* expected value abstractions */
void compute_expected_value(Hand * );

/* exchange turns */
void exchange_turn(Player * , int , Deck * );

/* helpers */
void clear_screen();

#endif



