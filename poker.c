#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poker.h"
#include "test.h"
#include <locale.h> 
#include <wchar.h> 
#define RUN_TESTS 0
#define TRUE 1
#define FALSE 0

void build_deck(Deck * deck){
	int i, j, c;
	c = 0;
	for (i = 0; i < SUITE_SIZE; ++i){
		for (j = 0; j < VALUE_SIZE; ++j) {
			deck->card[c].suite = i;
			deck->card[c].value = j;
			c = c + 1;
		}
	}
	deck->head = 0;
	deck->tail = 0;
	deck->size = DECK_SIZE;
}

void print_hand(Player * p){
	printf("%s : ", p->name);
	sort_cards(p->hand->card, HAND_SIZE);
	print(p->hand->card, HAND_SIZE);
}

void print(Card * deck, int size){
	int i;
	printf("[");
	for (i = 0; i < size; ++i){
		if (deck[i].value == -1){
			printf(" - ");
			continue;
		}
		printf("%s", to_card(deck[i]));
	}
	printf(" ] ");
}

char * to_card(Card card){
	char * result = malloc(sizeof(DECK_SIZE));
	sprintf(result, " %s%ls ", to_val(card.value),to_suite(card.suite));
	return result;
}

wchar_t * to_suite(int suite){
	wchar_t *clubs = L"\u2667"; 
    wchar_t *diamonds = L"\u2666"; 
    wchar_t *hearts = L"\u2665"; 
    wchar_t *spades = L"\u2664"; 
    setlocale(LC_ALL, ""); 
	
	switch(suite){
		case 0: return hearts; break;
		case 1: return clubs;  break;
		case 2: return spades;  break;
		case 3: return diamonds;  break;
		default: break;
	}
	return L"";
}

char * to_val(int value){
	if (value >= 0 && value <= VALUE_SIZE) {
	char * values[VALUE_SIZE] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    	return values[value];
    }
    return ""; /* error */
}

/* Knuth style shuffling */
void shuffle_deck(Deck * deck) {
	int i, index;
	for (i = 51; i >= 0; i--){
	    index = rand() % 51;
	    swap(&deck->card[i], &deck->card[index]);
	}
}

void deal_players(Player * players, Deck * deck){
	int i,j;
	Card card;
	for (i = 0; i < PLAYER_SIZE; ++i){
		for (j = 0; j < HAND_SIZE; ++j){
			card = deal_card(deck);
			players[i].hand->card[j] = card;
		}
	}
}

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

void swap(Card * d1, Card * d2){
	Card * temp = malloc(sizeof(Card));
    memcpy(temp, d1, sizeof(Card));
    memcpy(d1,d2, sizeof(Card));
    memcpy(d2,temp, sizeof(Card));
    free(temp);
}
	
Card deal_card(Deck * deck){ /* dequeue */
	Card return_card;
	Card temp;
	return_card.suite = 0;
	return_card.value = 0;
	if (deck->size == 0) return return_card; /* deck empty can't fill */

	temp = deck->card[deck->head];
	deck->card[deck->head].suite = -1; 
	deck->card[deck->head].value = -1;
	return_card = temp;
	deck->size--;
	deck->head++;
	if (deck->head == DECK_SIZE)
		deck->head = 0;
	return return_card;
}

void add_card(Deck * deck, Card card){ /* enqueue */
	if (deck->size == DECK_SIZE)
		return; /* can't add_card to a full deck */
	deck->card[deck->tail] = card;
	deck->tail = (deck->tail + 1) % DECK_SIZE;
	deck->size++;
	return;
}

void init_players(Player * players, int size){
	int i;
	for (i = 0; i < size; ++i){
		players[i].hand = malloc(sizeof(Hand));
		players[i].bet = 0;
		players[i].name = to_player(i+1);
		players[i].isComputer = 1;
		players[i].isActive = 1;
		players[i].amount = TOTAL_CASH / size;
		players[i].isBetter = 0;
	}
	players[0].isComputer = 0;
	players[0].name = "HU - Player 1 ";
	players[0].isBetter = 0;
}

char * to_player(int i){
	char * result = malloc(sizeof(30));
	sprintf(result, "AI - Player %d ",  i);
	return result;
}

void hand_value(Hand * hand){
	hand->value = (13 * hand->_class) + hand->rank;
}

void best_hand(Hand * hand, int isFinalRound, int isCPU){
	int i, oneS, twoS, threeS, fourS, fiveS, oneV, twoV, threeV, fourV, fiveV;
	sort_cards(hand->card, HAND_SIZE);
	if (isFinalRound || !isCPU) print(hand->card, HAND_SIZE);

	oneS   = hand->card[0].suite; oneV = hand->card[0].value;
	twoS   = hand->card[1].suite; twoV = hand->card[1].value;
	threeS = hand->card[2].suite; threeV = hand->card[2].value;
	fourS  = hand->card[3].suite; fourV = hand->card[3].value;
	fiveS  = hand->card[4].suite; fiveV = hand->card[4].value;

	/* Royal Flush */
	/* has a class rank of 10 */
	if ((oneS == twoS && twoS == threeS && threeS == fourS && fourS == fiveS) &&
		(oneV == Ten && twoV == Jack && threeV == Queen && fourV == King && fiveV == Ace)){
				
			hand->_class = 9;
			hand->rank = fiveV;
			hand->highCard.value = fiveV;
			hand->highCard.suite = fiveS;

			hand_value(hand);

			if (isFinalRound || !isCPU) printf("Royal Flush\n");

			return;
	}

	/* Straight Flush */
	/* has a class rank of 9 */
	if (oneS == twoS && twoS == threeS && threeS == fourS && fourS == fiveS && 
		fiveV == fourV + 1 && fourV == threeV + 1 && threeV == twoV + 1 && twoV == oneV + 1){
		hand->highCard.value = fiveV;
		hand->highCard.suite = fiveS;
		if (isFinalRound || !isCPU) printf("Straight Flush\n"); 
		hand->_class = 9;
		hand->rank = fiveV;
		hand_value(hand);
		return; 
	}

	/* Four-of-a-Kind */
	/* has a class rank of 8 */
	if ((oneV == twoV && twoV == threeV && threeV == fourV) || 
		(twoV == threeV && threeV == fourV && fourV == fiveV)){
			hand->highCard.value = fiveV;
			hand->highCard.suite = fiveS;
			if (isFinalRound || !isCPU) printf("Four of a kind\n");
			hand->_class = 8;
			hand->rank = threeV;
			hand_value(hand);
			return;
	}	

	/* Full House */
	/* has a class of 7 */
	if ((oneV == twoV && twoV == threeV && fourV == fiveV) ||
	    (oneV == twoV && threeV == fourV && fourV == fiveV)) {
			hand->highCard.value = fiveV;
			hand->highCard.suite = fiveS;
			if (isFinalRound || !isCPU) printf("Full House\n");
			hand->_class = 7;
			hand->rank = fiveV;
			hand_value(hand);
			return;
	}

	/* Flush */
	/* has a class of 6 */
	if (oneS == twoS && twoS == threeS && threeS == fourS && fourS == fiveS){
		hand->highCard.value = fiveV;
			hand->highCard.suite = fiveS;
			if (isFinalRound || !isCPU)printf("Flush\n");
			hand->_class = 6;
			hand->rank = fiveV;
			hand_value(hand);
		return; 
	}


	/* Straight */
	/* has a class of 5 */
	if (fiveV == fourV+1 && fourV+1 == threeV+2 && threeV+2 == twoV+3 && twoV+3 == oneV+4){
		hand->highCard.value = fiveV;
		hand->highCard.suite = fiveS;
		if (isFinalRound || !isCPU) printf("Straight\n");
		hand->_class = 5;
		hand->rank = fiveV;
		hand_value(hand);
		return; 
	}

	/* Three-of-a-Kind */
	/* has a class rank of 4 */
	for (i = 0; i < HAND_SIZE - 3; ++i){
		if (hand->card[i].value == hand->card[i+1].value && hand->card[i+1].value == hand->card[i+2].value){
			hand->highCard.value = fiveV;
			hand->highCard.suite = fiveS;
			hand->_class = 4;
			hand->rank = threeV;
			hand_value(hand);
			if (isFinalRound || !isCPU) printf("3 of a kind\n"); 
			return;
		}
	}

	/* Two-Pair */
	/* has a class rank of 3 */
	if ((oneV == twoV && threeV == fourV) ||
		(twoV == threeV && fourV == fiveV) ||
		(oneV == twoV && fourV == fiveV)) {
			hand->highCard.value = fiveV;
			hand->highCard.suite = fiveS;
			if (isFinalRound || !isCPU) printf("Two Pair\n"); 
			hand->_class = 3;
			hand->rank = fourV;
			hand_value(hand);
			return;
	}

	/* One-Pair */
	/* has a class rank of 2 */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		int j, value;
		if (hand->card[i].value == hand->card[i+1].value){
			value = hand->card[i].value;
			if (isFinalRound || !isCPU) {
				printf("Pair (");
				for (j = 0; j < 2; ++j){
					printf("%s", to_card(hand->card[i]));
					i++;
				}
				printf(")\n");
			}
			hand->_class = 2;
			hand->highCard.value = fiveV;
			hand->highCard.suite = fiveS;
			hand->rank = value;
			hand_value(hand);
			return;
		}
	}
	/* High Card */
	/* has a class rank of 1 */
	if (isFinalRound || !isCPU)
		printf("High - Card %s%ls\n", to_val(fiveV), to_suite(fiveS));
	hand->_class = 1;
	hand->highCard.value = fiveV;
	hand->highCard.suite = fiveS;
	hand->rank = fiveV;
	hand_value(hand);
	return;
	
}

void compute_expected_value(Hand * hand) {

	/* discard none (5 choose 0) == 1 */

		/* 1 */

	/* discard one (5 choose 1) == 5 */

		/* 1,2,3,4,5 */

	/* discard two (5 choose 2) == 10 */

		/* (1,2)(1,3)(1,4)(1,5)(2,3)(2,4)(2,5)(3,4)(3,5)(4,5)*/

	/* discard three (5 choose 3) == 10 */

		/* lots... */

	/* discard four (5 choose 4) == 5 */

		/* (1,2,3,4)-(1,2,3,5)-(1,2,4,5)-(1,3,4,5)-(2,3,4,5) */

	/* discard four (5 choose 5) == 1 */

		/* (1,2,3,4,5) */

	int i, j, size, k, l, z, y;
	size = 1;

	int cases[32][5] = {

		 {0,0,0,0,0}, /* first case */
	     
	     {1,0,0,0,0}, /* second case */
	     {0,1,0,0,0},
	     {0,0,1,0,0},
	     {0,0,0,1,0},
	     {0,0,0,0,1},

	     {1,1,0,0,0}, /* third case */
	     {1,0,1,0,0},
	     {1,0,0,1,0},
	     {1,0,0,0,1},
	     {0,1,1,0,0},
	     {0,1,0,1,0},
	     {0,1,0,0,1},
	     {0,0,1,1,0},
	     {0,0,1,0,1},
	     {0,0,0,1,1},	

	     {1,1,1,0,0}, /* fourth cases */
	     {1,1,0,1,0},
	     {1,1,0,0,1},
	     {1,0,0,1,1},
	     {1,0,1,0,1},
	     {1,0,1,1,0},
	     {0,1,1,1,0},
	     {0,1,0,1,1},
	     {0,1,1,0,1},
	     {0,0,1,1,1},

	     {1,1,1,1,0}, /* fourth case */
	     {1,1,1,0,1},
	     {1,1,0,1,1},
	     {1,0,1,1,1},
	     {0,1,1,1,1},

	     {1,1,1,1,1}, /* last case */
	
	 };

	Exchange * e = malloc(sizeof(Exchange));
	Exchange * tempEx = malloc(sizeof(Exchange));
	Deck * deck = malloc(sizeof(Deck));
	Card * temp = malloc(sizeof(Card) * HAND_SIZE);
	int expected_array[32];

	/* init our array */
	for (i = 0; i < 32; ++i) expected_array[i] = 0;

	printf("Computing expected value via Monte Carlo simulation\n");

	/* first case 5 choose 0 */
	/* first value of expected array is the expected value of doing nothing, 
	in which I just fill the array with the value of the hand */

	build_deck(deck);
	shuffle_deck(deck);	
	memcpy(temp, &hand->card, sizeof(Card) * HAND_SIZE); /* hold on to this for restore */	
	for (i = 0; i < 32; i++){
			l = 0;
			for (k = 0; k < HAND_SIZE; ++k){
				if (cases[i][k]){
					e->card[l] = hand->card[k];
					l += 1;
					e->count = l;
					hand->card[k].suite = -1; /* set card to empty */
					hand->card[k].value = -1;
				}
			}

			if (l == 0) { /* this is for our first case only */
				best_hand(hand, TRUE, TRUE);
				/*printf("%d\n", hand->value);*/
				expected_array[i] = hand->value;
				memcpy(&hand->card, temp, sizeof(Card) * HAND_SIZE); /* restore hand */
				continue;
			}

			for (j = 0; j < size; j++){
				print(hand->card, HAND_SIZE);
				print(e->card, l);
				memcpy(tempEx, e, sizeof(Exchange));
				exch:
					exchange_card(deck, e, l); /* exchange with the deck */
					for (z = 0; z < l; ++z){ /* make sure it doesnt already exist in the hand */
						for (y = 0; y < HAND_SIZE; y++){ /* if it does, try the exch again */
							if ((e->card[z].suite == temp[y].suite) && 
								(e->card[z].value == temp[y].value)){
									/*printf("Already exists: %s !!\n", to_card(e->card[z]));*/
									memcpy(e, tempEx, sizeof(Exchange));
									goto exch;
							}
						}
					}

				/*printf("exchange \n");*/
				print(e->card, l);
				insert_exchange_into_hand(e, hand, l); /* insert the exchange back into the hand */
				best_hand(hand, TRUE, TRUE);
				/*printf("%d\n", hand->value);*/
				expected_array[i] += hand->value;
				memcpy(&hand->card, temp, sizeof(Card) * HAND_SIZE); /* restore hand */
				build_deck(deck);
				shuffle_deck(deck);	
		}
		expected_array[i] = expected_array[i] / size; /* average */
	}


	for (i = 0; i < 32; ++i)
		printf("Expected value: %d - %d\n", i, expected_array[i]);

	j = k = 0;
	for (i = 0; i < 32; ++i)
		if (j < expected_array[i]){
			j = expected_array[i];
			k = i;
		}

	printf("largest: %d at %d\n", j, k);


	/* MC recommendation */
	if (k == 0)
		printf("MC: I recommend that you keep your current hand\n");
	else if (k > 0){ 
		printf("MC: I recommend that you exchange cards: ");
		for (i = 0; i < 5; ++i){
			if (cases[k][i])
				printf("%d ", i);
		}
		printf("\n");
	}
}

void init_deck(Deck * deck){
	build_deck(deck);
	shuffle_deck(deck);
}

void show_hands(Player * players, int player_count, int lastRound){
	int i;
	for (i = 0; i < player_count; ++i) { 
		if ((players[i].isActive && !players[i].isComputer) || lastRound){
			printf("%s", players[i].name);
			best_hand(players[i].hand, lastRound, players[i].isComputer);
		}
	}
}


void reset_game(Game * g){
	g = malloc(sizeof(Game));
	init_game(g);
}

void init_game(Game * game){
	game->deck = malloc(sizeof(Deck));
	game->players = malloc(sizeof(Player) * PLAYER_SIZE);
	game->value = TOTAL_CASH;
	game->pot = 0;
	game->ante = ANTE_AMOUNT;
	game->player_count = PLAYER_SIZE;
	init_deck(game->deck);
	init_players(game->players, PLAYER_SIZE);
}

void betting_round(Game * game, Player * players, int player_size){
	int i;
	i = 0;
	char * result = malloc(sizeof(char) * 10);
	int bet;

	/* clear previous better */
	for (i = 0; i < player_size; ++i)
		players[i].isBetter = 0;

	/* determine new better, randomly */
	find_better:
		i = rand() % 4;
		if (!players[i].isActive)
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
			if (!players[i].hasFolded){
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
	free(result);
	printf("Current pot at: %d\n", game->pot);
}

void exchange_turn(Player * players, int player_size, Deck * deck){
	int i;
	for (i = 0; i < player_size; ++i) 
		prompt_player(&players[i], deck);
}

void clear_screen(){ printf("\e[1;1H\e[2J"); }

void ante_up(Game * game){
	int i;
	for (i = 0; i < game->player_count; ++i){
		if (game->players[i].isActive){
			if ((game->players[i].amount - game->ante) < 0){
				printf("%s loses, not enough money to play\n", game->players[i].name);
				game->players[i].isActive = 0;
				game->pot += game->players[i].amount; /* leftovers go into pot */
				game->players[i].amount = 0;
				game->player_count -= 1;
				continue;
			}
			game->pot += game->ante; /* increase pot */
			game->players[i].amount -= game->ante; /* decrease player pot */
			printf("%s ante's %d, now has: %d chips\n", game->players[i].name, game->ante, game->players[i].amount);
		}
	}
	printf("pot at: %d\n", game->pot);
}

int game_winner(Game * game, int psize){
	int i;
	char * result = malloc(sizeof(char) * 2);
	for (i = 0; i < psize; ++i){
	 	if (game->players[i].amount == TOTAL_CASH){
	 		printf("%s is the winner. \n", game->players[i].name);
	 		printf("Would you like to play again? 'y' or 'n'\n");
	 		if (!strncmp(result, "y", strlen("y"))){
	 			reset_game(game);
	 			return 0;
	 		}
	 		return 1;
	 	}
	 } 
	 return 0;
}

int start_game() {
	
    if (RUN_TESTS) return exp_value();  /* enable and disables tests */ 
	
	Game * game = malloc(sizeof(Game));
	init_game(game);

	while (!game_winner(game, game->player_count)) {
		
		print(game->deck->card, game->deck->size);
		printf("\n");

		/* deal players */
		deal_players(game->players, game->deck);

		/* ante up */
		ante_up(game);

		/* show hands */
		show_hands(game->players, game->player_count, FALSE);

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


void prompt_player(Player * p, Deck * d){
	Card temp;
	char * result = malloc(sizeof(char)*10);
	int i, j, k; 
	Exchange * e = malloc(sizeof(Exchange));

	i = j = 0;
	if (!p->hasFolded){
		if (p->isComputer){
			/* insert monte carlo decision analyzer here */
			/* in the meantime just select a # at random */

			j = 1;
			k = rand() % 5;

			temp = p->hand->card[k]; 
			p->hand->card[k].suite = -1;
			p->hand->card[k].value = -1;
			e->card[0] = temp;

			/*printf("%s Exchanged: ", p->name);
			for (i = 0; i < j; ++i){	
				printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
			}
			printf(" For: ");*/
			exchange_card(d, e, j);
			/*for (i = 0; i < j; ++i){
				printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
			}*/

			insert_exchange_into_hand(e, p->hand, j);
			/*printf(" New hand: ");*/
			best_hand(p->hand, FALSE, p->isComputer);

			return;

		}

		/* monte carlo helper */
		compute_expected_value(p->hand);

		printf("%s, Which cards would you like to exchange? (ex: '1,2,3' or '1,5 etc..')\n", p->name);


		scanf("%s", result);
		while (result[i] != '\0'){
			if (result[i] == ','){
				i++;
				continue;
			}

			k = atoi(&result[i]);
			k = k-1;

			/* take card out of hand and put it into the exchange struct */
			temp = p->hand->card[k]; 
			p->hand->card[k].suite = -1;
			p->hand->card[k].value = -1;	
			e->card[j] = temp;
			j++; 
			i++;
		}
		if (!p->isComputer){
			printf("%s Exchanged: ", p->name);
			for (i = 0; i < j; ++i){
				printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
			}
			printf(" For: ");
		}
		
		exchange_card(d, e, j);
		if (!p->isComputer){
			for (i = 0; i < j; ++i){
				printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
			}
		}
		insert_exchange_into_hand(e, p->hand, j);
		if (!p->isComputer){
			printf(" New hand: ");
		}
		best_hand(p->hand, FALSE, p->isComputer);
		return;
	}

}

void determine_winner(Game * game, Player * players, int p_size){
	int max, i, counter, rem;
	max = counter = rem = 0;

	char * result = malloc(sizeof(char));
	char * split = malloc(sizeof(int)*4);

	/* display hands on last round */
	show_hands(players, game->player_count, TRUE);


	for (i = 0; i < 4; ++i)
		split[i] = -1;

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
			if (game->pot % counter != 0){ /* if pot is uneven i.e. pot is 32 with a 3 way tie */
				rem = game->pot % counter;
				game->pot -= rem;
				for (i = 0; i < p_size; ++i){
					if (split[i] != -1)
						players[i].amount += game->pot / counter;
				}
				game->pot = rem;
			}
			else {
				for (i = 0; i < p_size; ++i)
					if (split[i] != -1)
						players[i].amount += game->pot / counter;
				}
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
	

	/*rest round */
	for (i = 0; i < p_size; ++i){
		if (players[i].isActive){
			players[i].hasFolded = 0;
		}
	}

	/* Rankings */
	for (i = 0; i < p_size; ++i){
		if (players[i].isActive){
			printf("%s - Score: %d, Chips: %d\n", 
			players[i].name, 
			players[i].hand->value,
			players[i].amount);
		}
	}
	
	printf("Press enter to start next round:\n");
	free(result);
}

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


void sort_cards(Card * cards, int size){
	qsort(cards, size, sizeof(*cards), card_comparator);
}

int card_comparator(const void * a, const void * b){
    const Card * c1 = (const Card *)a;
    const Card * c2 = (const Card *)b;
    return (c1->value - c2->value);
}