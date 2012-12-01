#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poker.h"
#include <locale.h> 
#include <wchar.h> 

void build_deck(Deck * deck){
	int i, j, c;
	srand(time(NULL));
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
	for (i = 0; i < size; ++i)
		printf("%s%ls ", to_val(deck[i].value), to_suite(deck[i].suite));
	
	printf("\n");
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

Exchange exchange_card(Deck * deck, Exchange e, int count){
	Card card;
	int i;
	for (i = 0; i < count; ++i){
		card = deal_card(deck);
		add_card(deck, e.card[i]);
		e.card[i] = card;
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
	deck->card[deck->head].suite = 0; 
	deck->card[deck->head].value = 0;
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
	for (i = 0; i < PLAYER_SIZE; ++i){
		players[i].hand = malloc(sizeof(Hand));
		players[i].bet = 0;
		players[i].name = to_player(i+1);
		players[i].isComputer = 1;
	}
	players[0].isComputer = 0;
	players[0].name = "Human - Player 1";
}

char * to_player(int i){
	char * result = malloc(sizeof(30));
	sprintf(result, "AI - Player %d", i);
	return result;
}

void best_hand(Card * hand){
	int i, result, oneS, twoS, threeS, fourS, fiveS, oneV, twoV, threeV, fourV, fiveV;
	sort_cards(hand, HAND_SIZE);
	print(hand, HAND_SIZE);

	oneS   = hand[0].suite; oneV = hand[0].value;
	twoS   = hand[1].suite; twoV = hand[1].value;
	threeS = hand[2].suite; threeV = hand[2].value;
	fourS  = hand[3].suite; fourV = hand[3].value;
	fiveS  = hand[4].suite; fiveV = hand[4].value;

	/* Royal Flush */
	if ((oneS == twoS && twoS == threeS && threeS == fourS && fourS == fiveS) &&
		(oneV == Ten && twoV == Jack && threeV == Queen && fourV == King && fiveV == Ace)){
				printf("Royal Flush\n");
				return;
		}

	/* Straight Flush */
	result = 0;
	if (oneS == twoS && twoS == threeS && threeS == fourS && fourS == fiveS && 
		fiveV == fourV + 1 && fourV == threeV + 1 && threeV == twoV + 1 && twoV == oneV + 1){
		printf("Straight Flush\n"); 
		return; 
	}

	/* Four-of-a-Kind */
	if ((oneV == twoV && twoV == threeV && threeV == fourV) || 
		(twoV == threeV && threeV == fourV && fourV == fiveV)){
			printf("Four of a kind\n");
			return;
	}	

	/* Full House */
	if ((oneV == twoV && twoV == threeV && fourV == fiveV) ||
	    (oneV == twoV && threeV == fourV && fourV == fiveV)) {
			printf("Full House\n");
			return;
	}

	/* Flush */
	if (oneS == twoS && twoS == threeS && threeS == fourS && fourS == fiveS){
		printf("Flush\n");
		return; 
	}


	/* Straight */
	if (fiveV == fourV+1 && fourV+1 == threeV+2 && threeV+2 == twoV+3 && twoV+3 == oneV+4){
		printf("Straight\n");
		return; 
	}

	/* Three-of-a-Kind */
	result = 0;
	for (i = 0; i < HAND_SIZE - 3; ++i){
		if (hand[i].value == hand[i+1].value && hand[i+1].value == hand[i+2].value){
			printf("3 of a kind\n"); return;
		}
	}

	/* Two-Pair */
	result = 0;
	if (oneV == twoV && threeV == fourV) result+=1;
	if (twoV == threeV && fourV == fiveV) result+=1;
	if (oneV == twoV && fourV == fiveV) result+=1;
	if (result > 0) {printf("Two Pair\n"); return;}

	/* One-Pair */
	for (i = 0; i < HAND_SIZE-1; ++i) {
		if (hand[i].value == hand[i+1].value){
			printf("Pair\n");
			return;
		}
	}

	printf("High - Card %s%ls\n", to_val(fiveV), to_suite(fiveS));

	/* High Card */
	return;
	
}

void prompt_player(Player * p, Deck * d){
	Card temp;
	char * result = malloc(sizeof(char)*10);
	int i, j, k; 
	Exchange e;

	i = j = 0;

	if (p->isComputer){
		/* monte carlo this shit */



		return;

	}

	printf("Which cards would you like to exchange? (ex: '1,2,3' or '1,5')\n");

	scanf("%s", result);
	while (result[i] != '\0'){
		if (result[i] == ','){
			i++;
			continue;
		}
		k = atoi(&result[i]);
		/* take card out of hand and put it into the exchang struct */
		temp = p->hand->card[k]; 
		p->hand->card[k].suite = 0;
		p->hand->card[k].value = 0;
		e.card[j] = temp;
		j++; 
		i++;
	}
	printf("Exchanging...");
	exchange_card(d, e, j);
	printf("Received: \n");
	for (i = 0; i < j; ++i)
	{
		print(e.card, j);
	}
	insert_exchange_into_hand(&e, p->hand, j);
	printf("new hand:");
	print(p->hand->card, HAND_SIZE);
	best_hand(p->hand->card);
}

void insert_exchange_into_hand(Exchange*e, Hand*h, int size){
	int i;
	Card temp;
	for (i = 0; i < size; ++i)
	{
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






