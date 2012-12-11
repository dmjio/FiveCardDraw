#include <stdio.h>
#include "hand.h"

void hand_value(Hand * hand){
	hand->value = (13 * hand->_class) + hand->rank;
}

void best_hand(Hand * hand, int isFinalRound, int isCPU){

			/* this method is a fall through that 
				calculates hand value and 
				prints results of hand, it works by sorting the hand and then 
				making comparison to derive the value of the hand */


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
	for (i = 0; i < HAND_SIZE - 2; ++i){
		if ((hand->card[i].value == hand->card[i+1].value) && (hand->card[i+1].value == hand->card[i+2].value)){
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
