#include "card.h"

/* card helper */
char * to_card(Card card){
	char * result = malloc(sizeof(DECK_SIZE));
	sprintf(result, " %s%ls ", to_val(card.value),to_suite(card.suite));
	return result;
}

/* card helper to display card value */
char * to_val(int value){
	if (value >= 0 && value <= VALUE_SIZE) {
	char * values[VALUE_SIZE] = { "2", "3", "4", "5", "6", "7", "8", "9", 
									"10", "J", "Q", "K", "A" };
    	return values[value];
    }
    return ""; /* error */
}

/* card helper to display suite */
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

/* helper to print the deck or a hand */
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

/* helper to sort cards */
void sort_cards(Card * cards, int size){
	qsort(cards, size, sizeof(*cards), card_comparator);
}
/* comparator function for the sort_cards qsort method, note I am only valuing card value, not suite */
/* if a tie occurs then the pot is split, I don't check "highest" suite */
int card_comparator(const void * a, const void * b){
    const Card * c1 = (const Card *)a;
    const Card * c2 = (const Card *)b;
    return (c1->value - c2->value);
}