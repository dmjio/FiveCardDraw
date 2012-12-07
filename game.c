#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "game.h"

/* method for resetting game loop */
void reset_game(Game * g){
	free(g);
	Game * game = malloc(sizeof(Game));
	init_game(game);
}

/* this method begins the game */
void init_game(Game * game){
	printf("Welcome to 5-card draw Poker with Monte Carlo simulations!\n");
	game->deck = malloc(sizeof(Deck));
	game->players = malloc(sizeof(Player) * PLAYER_SIZE);
	game->value = TOTAL_CASH;
	game->pot = 0;
	game->ante = ANTE_AMOUNT;
	game->player_count = PLAYER_SIZE;
	init_deck(game->deck);
	init_players(game->players, PLAYER_SIZE);
}

/* determines whether a user is eligible to play, if they can't ante up, they lose */
void ante_up(Game * game){
	int i, count;
	count = 0;
	for (i = 0; i < game->player_count; ++i){
		if (game->players[i].isActive){
			if ((game->players[i].amount - game->ante) < 0){
				printf("%s loses, not enough money to play\n", game->players[i].name);
				game->players[i].isActive = 0;
				game->players[i].hasFolded = 1;
				game->pot += game->players[i].amount; /* leftovers go into pot */
				game->players[i].amount = 0;
				count++;
				continue;
			}
			game->pot += game->ante; /* increase pot */
			game->players[i].amount -= game->ante; /* decrease player pot */
			printf("%s ante's %d, now has: %d chips\n", game->players[i].name, game->ante, game->players[i].amount);
		}
	}
	printf("Pot at: %d\n", game->pot);
}

/* called twice, once before exchange, once after */
void betting_round(Game * game, Player * players, int player_size){
	int i;
	i = 0;
	char * result = malloc(sizeof(char) * 10);
	int bet;
	int count;

	/* check to make sure everyone didn't fold */
	/* if everyone has folded */
	count = 0;
	for (i = 0; i < PLAYER_SIZE; ++i)
		if (players[i].isActive && players[i].hasFolded)
			count++;

	if (player_size - count != 1 && game->pot != TOTAL_CASH){ /* everyone has folded go straight to determining winner */
		/* clear previous better */
		for (i = 0; i < player_size; ++i)
			players[i].isBetter = 0;

		/* determine new better, randomly */
		find_better:
			i = rand() % 4;
			if (!players[i].isActive || players[i].hasFolded)
				goto find_better;
			players[i].isBetter = 1;
				
		/* make bets */
			for (i = 0; i < player_size; ++i) {
				if (players[i].isComputer && players[i].isBetter){
						bet = rand() % players[i].amount;
						printf("%s bet %d\n", players[i].name, bet);
						players[i].amount -= bet;
						game->pot += bet;

				}
				if (!players[i].isComputer && players[i].isBetter){
					printf("What would you like to bet? (ex: 1, 10, 50), you have: %d\n", players[i].amount);
				bet:
					scanf("%s", result);
					bet = atoi(result);
					if (bet > players[i].amount || bet < 0){
						printf("Too high or too low, please try again\n");
						goto bet;
					}
					printf("%s bet %d\n", players[i].name, bet);
					players[i].amount -= bet;
					game->pot += bet;
				}
			}
			/* if player is not better, he must meet the bet or fold */
			for (i = 0; i < player_size; ++i) {
				if (!players[i].hasFolded && players[i].isActive){
					if (players[i].isComputer && !players[i].isBetter){
						if (players[i].amount - bet < 0) {
							players[i].hasFolded = 1;
							printf("%s has folded\n", players[i].name);
						}
						else {
							players[i].amount -= bet;
							printf("%s called the bet\n", players[i].name);
							game->pot += bet;
						}
					}
					if (!players[i].isComputer && !players[i].isBetter){
						if (players[i].amount - bet < 0) {
							players[i].hasFolded = 1;
							printf("%s has folded\n", players[i].name);
						}
						else {
						printf("What would you like to do? Call or Fold? (ex: 'c' or 'f') \n");
						scanf("%s", result);
						if (!strncmp(result, "c", strlen("c"))){
							players[i].amount -= bet;
							printf("%s called the bet\n", players[i].name);
							game->pot += bet;
						}
						else {
							players[i].hasFolded = 1;
							printf("%s has folded\n", players[i].name);
						}
					}
				}
			}
		}
		printf("Current pot at: %d\n", game->pot);
	}

	free(result);
}


/* determines winner */
void determine_winner(Game * game, Player * players, int p_size){
	int max, i, counter, rem;
	max = counter = rem = 0;
	char * result = malloc(sizeof(char));
	char * split = malloc(sizeof(int)*4);

	/* display hands on last round */
	show_hands(players, game->player_count, TRUE);

	for (i = 0; i < 4; ++i) split[i] = -1;

	/* get best hand value for all players who are active and who have not folded */
	for (i = 0; i < p_size; ++i){
		if (players[i].isActive && !players[i].hasFolded){
			if (max < players[i].hand->value){
					max = players[i].hand->value;
				}
			}
		}

	/* split the pot if more than one winner */
	for (i = 0; i < p_size; ++i)
		if (players[i].isActive && !players[i].hasFolded)
			if (max == players[i].hand->value)
				split[i] = max;

	for (i = 0; i < p_size; ++i)
		if (split[i] > -1)
			counter++;

	/* split pot, more than one winner */
	if (counter > 1){
			printf("Tie game, split the pot\n");
			for (i = 0; i < p_size; ++i)
				if (split[i] != -1) 
					printf("%s wins round.\n", players[i].name);

			if (game->pot % counter != 0){ /* if pot is uneven i.e. pot is 32 with a 3 way tie */
				rem = game->pot % counter;
				game->pot -= rem;
				for (i = 0; i < p_size; ++i){
					if (split[i] != -1)
						players[i].amount += game->pot / counter;
				}
				game->pot = rem;
			} else {
				for (i = 0; i < p_size; ++i)
					if (split[i] != -1)
						players[i].amount += game->pot / counter;

				}
				game->pot = 0;
			}
	else { /* not a split pot */
		for (i = 0; i < p_size; ++i){
			if (players[i].isActive && !players[i].hasFolded){
				if (players[i].hand->value == max){
					printf("%s wins round.\n", players[i].name);
					players[i].amount += game->pot;
				}
			}
		}
		game->pot = 0;
	}

	/* Rankings */
	for (i = 0; i < p_size; ++i){
		if (players[i].isActive){
			printf("%s - Score: %d, Chips: %d", 
			players[i].name, 
			players[i].hand->value,
			players[i].amount);
		}
		if (players[i].isActive && players[i].hasFolded)
			printf(" (Folded)");
		printf("\n");
	}

	for (i = 0; i < p_size; ++i){
		if (players[i].isActive){
			players[i].hasFolded = 0;
		}
	}
	
	printf("Type any key and enter to start next round: ");
	scanf("%s", result);
	if (!strncmp(result, "", strlen(""))) /* dummy */
		clear_screen();

	printf("\n");
		
	free(result);
	free(split);
}

int game_winner(Game * game, int psize){
	int i;
	char * result = malloc(sizeof(char));
	for (i = 0; i < psize; ++i){
	 	if (game->players[i].amount == TOTAL_CASH){
	 		printf("%s is the winner. \n", game->players[i].name);
	 		printf("Would you like to play again? 'y' or 'n'\n");
	 		scanf("%s", result);
	 		if (!strncmp(result, "y", strlen("y"))){
	 			clear_screen();
	 			reset_game(game);
	 			return 0;
	 		}
	 		return 1;
	 	}
	 } 
	 init_deck(game->deck); /* reset deck */
	 return 0;
}
