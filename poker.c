#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poker.h"

/* this method aggregates everything and is where the core high level game logic exists */
int start_game() {

    if (RUN_TESTS) return test_queue();  /* enable and disables tests */ 
	Game * game = malloc(sizeof(Game));
	init_game(game);

	while (!game_winner(game, game->player_count)) {
		
		/* deal players */
		deal_players(game->players, game->deck);

		/* ante up */
		ante_up(game);

		/* show hand to human player (cpu show is disabled, enable it to see some action) */
		show_hands(game->players, game->player_count, TRUE);

		/* first round of betting */
		betting_round(game, game->players, game->player_count);

		/* players exchange cards if any */
		exchange_turn(game->players, game->player_count, game->deck);

		/* second round of betting */
		betting_round(game, game->players, game->player_count);

		/* print winning hand and player */
		determine_winner(game, game->players, game->player_count);
	}

	return 0;

}







