#include "card.h"

char * to_card(Card card){
	char * result = malloc(sizeof(DECK_SIZE));
	sprintf(result, " %s%ls ", to_val(card.value),to_suite(card.suite));
	return result;
}

char * to_val(int value){
	if (value >= 0 && value <= VALUE_SIZE) {
	char * values[VALUE_SIZE] = { "2", "3", "4", "5", "6", "7", "8", "9", 
									"10", "J", "Q", "K", "A" };
    	return values[value];
    }
    return ""; /* error */
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


void sort_cards(Card * cards, int size){
	qsort(cards, size, sizeof(*cards), card_comparator);
}

int card_comparator(const void * a, const void * b){
    const Card * c1 = (const Card *)a;
    const Card * c2 = (const Card *)b;
    return (c1->value - c2->value);
}