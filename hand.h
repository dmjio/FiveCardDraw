#ifndef __HAND_H_
#define __HAND_H_
#define HAND_SIZE 5

#include "card.h"

typedef struct {
	Card card[HAND_SIZE];
	Card highCard;
	int rank;
	int _class;
	int value;
	void (*calc_hand_value)(int _class, int rank);
} Hand;

void hand_value(Hand * hand);
void best_hand(Hand * hand, int isFinalRound, int isCPU);


#endif 