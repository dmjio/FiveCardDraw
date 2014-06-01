#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "montecarlo.h"
#include "deck.h"
#include "hand.h"
#include "exchange.h"

#define TOTAL_CASH 400
#define PLAYER_SIZE 4
#define ONLYCPU 1

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

/* player abstractions */
void build_players(Player *);
void deal_players(Player *, Deck *);
void prompt_player(Player * , Deck * );
void show_hands(Player * players, int player_count, int lastRound);
void exchange_turn(Player *,  int,  Deck * );


/* players */
void init_players(Player * , int);
char * to_player(int );
void print_hand(Player * );

#endif 
