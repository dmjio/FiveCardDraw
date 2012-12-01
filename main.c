#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> 
#include "poker.h"
#include "test.h"
#define RUN_TESTS 0;

int main(int argc, char const *argv[])
{
	 
	int i;
	Deck * deck = malloc(sizeof(Deck));
	Player * players = malloc(sizeof(Player) * PLAYER_SIZE);

	build_deck(deck);
	shuffle_deck(deck);

	/* build players */
	init_players(players, 4);
	deal_players(players, deck);
	
	for (i = 0; i < PLAYER_SIZE; ++i) { 
		best_hand(players[i].hand->card);
	}

	for (i = 0; i < PLAYER_SIZE; ++i) { 
		prompt_player(&players[i], deck);
	}

	/* print hands */

	return 0;
}