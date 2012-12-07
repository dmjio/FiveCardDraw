#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"


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

void show_hands(Player * players, int player_count, int lastRound){
	int i;
	for (i = 0; i < player_count; ++i) { 
		if (players[i].isActive){
			printf("%s", players[i].name);
			best_hand(players[i].hand, lastRound, players[i].isComputer);
		}
	}
}

/* this method prompts both the CPU and HU players on what to exchange */
/* cpu will always select the MC's recommendation */
/* HU's have a choice to reject or accept the MC's rec */
void prompt_player(Player * p, Deck * d){
	Card temp;
	int * recommendation = malloc(sizeof(int) * HAND_SIZE);
	char * result = malloc(sizeof(char)*10);
	int i, j, k; 
	Exchange * e = malloc(sizeof(Exchange));

	i = j = 0;
	if (!p->hasFolded && p->isActive){
		if (p->isComputer){

			/* monte carlo this ish! */
			recommendation = compute_expected_value(p->hand, p->isComputer);

			/* computer automatically takes the MC recommendations every time */
			if (recommendation[0] != -1){
				for (i = 0; i < HAND_SIZE; ++i)
					if (recommendation[i]){
						e->card[j++] = p->hand->card[i];
						p->hand->card[i].suite = -1;
						p->hand->card[i].value = -1;
					}
			
			/* if Human, display what was exchanged */
			printf("%s Exchanged: ", p->name);
			for (i = 0; i < j; ++i){	
				printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
			}
			printf("\n\t\t     For: ");
			exchange_card(d, e, j);
			for (i = 0; i < j; ++i){
				printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
			}

			/* make the exchange */
			insert_exchange_into_hand(e, p->hand, j);
			
			printf("\n\t\tNew hand: ");
		} else {
			/* if human */
			printf("%s Stayed: ", p->name);
		}
			best_hand(p->hand, FALSE, FALSE);
			printf("\n");
			free(result);
			free(e);
			free(recommendation);

			return;

		}

		/* monte carlo helper */
		recommendation = compute_expected_value(p->hand, p->isComputer);

		j = 0;
		scanf("%s", result);

		/* shortcut for not exchanging anything */
		if (!strncmp(result, "y", strlen("y"))){
			for (i = 0; i < HAND_SIZE; ++i){
				if (recommendation[0] == -1)
					goto do_nothing;
				if (recommendation[i]){
					e->card[j++] = p->hand->card[i];
					p->hand->card[i].suite = -1;
					p->hand->card[i].value = -1;
				}
			}
		} else {
			printf("%s, Which cards would you like to exchange? (ex: '1,2,3' or '1,5 etc..')\n", 
				p->name);

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
		}

		printf("%s Exchanged: ", p->name);
		for (i = 0; i < j; ++i){
			printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
		}
		printf("\n\t\t     For: ");
		
		exchange_card(d, e, j);
			for (i = 0; i < j; ++i){
				printf("%s%ls ", to_val(e->card[i].value), to_suite(e->card[i].suite));
			}
		insert_exchange_into_hand(e, p->hand, j);
		printf("\n\t\tNew hand: ");
	do_nothing:
		if (recommendation[0] == -1)
			printf("%s Stayed: ", p->name);
		best_hand(p->hand, FALSE, p->isComputer);
		printf("\n");

		free(result);
		free(e);
		free(recommendation);

		return;
	}

}

void exchange_turn(Player * players, int player_size, Deck * deck){
	int i, count;
	/* check to make sure everyone didn't fold */
	/* if everyone has folded */
	count = 0;
	for (i = 0; i < PLAYER_SIZE; ++i)
		if (players[i].isActive && players[i].hasFolded)
			count++;

	if (player_size - count != 1) /* everyone has folded skip */
		for (i = 0; i < player_size; ++i) 
			prompt_player(&players[i], deck);
}

char * to_player(int i){
	char * result = malloc(sizeof(30));
	sprintf(result, "AI - Player %d ",  i);
	return result;
}

void print_hand(Player * p){
	printf("%s : ", p->name);
	sort_cards(p->hand->card, HAND_SIZE);
	print(p->hand->card, HAND_SIZE);
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
	if (!ONLYCPU){
		players[0].isComputer = 0;
		players[0].name = "HU - Player 1 ";
		players[0].isBetter = 0;
	}
}