#ifndef __GAME_H_
#define __GAME_H_
#define ANTE_AMOUNT 5

#include "utils.h"
#include "deck.h"
#include "player.h"

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
void ante_up(Game * game);
void determine_winner(Game * , Player * , int );
void betting_round(Game * , Player * , int );


#endif
