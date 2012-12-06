
#include "utils.h"

void swap(Card * d1, Card * d2){
	Card * temp = malloc(sizeof(Card));
    memcpy(temp, d1, sizeof(Card));
    memcpy(d1,d2, sizeof(Card));
    memcpy(d2,temp, sizeof(Card));
    free(temp);
}

void clear_screen(){ printf("\e[1;1H\e[2J"); }

