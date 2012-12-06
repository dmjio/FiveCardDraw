#include "montecarlo.h"

int * compute_expected_value(Hand * hand, int isCPU) {

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
	size = NUMBER_OF_SAMPLES;
	int * result = malloc(sizeof(int) * 5);

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

	     {1,1,1,1,1}, /* last case, swap all */
	
	 };

	Exchange * e = malloc(sizeof(Exchange));
	Exchange * tempEx = malloc(sizeof(Exchange));
	Deck * deck = malloc(sizeof(Deck));
	Card * temp = malloc(sizeof(Card) * HAND_SIZE);
	int expected_array[32];

	/* cleanse our array */
	for (i = 0; i < 32; ++i) expected_array[i] = 0;

	if (!isCPU)
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
				best_hand(hand, FALSE, TRUE);
				/*printf("%d\n", hand->value);*/
				expected_array[i] = hand->value;
				memcpy(&hand->card, temp, sizeof(Card) * HAND_SIZE); /* restore hand */
				continue;
			}

			for (j = 0; j < size; j++){
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
				insert_exchange_into_hand(e, hand, l); /* insert the exchange back into the hand */
				best_hand(hand, FALSE, TRUE);
				/*printf("%d\n", hand->value);*/
				expected_array[i] += hand->value;
				memcpy(&hand->card, temp, sizeof(Card) * HAND_SIZE); /* restore hand */
				build_deck(deck);
				shuffle_deck(deck);	
		}
		expected_array[i] = expected_array[i] / size; /* average */
	}

	/* find largest value inside our expected values array and its index */
	j = k = 0;
	for (i = 0; i < 32; ++i)
		if (j < expected_array[i]){
			j = expected_array[i];
			k = i;
		}

	/*printf("largest: %d at %d\n", j, k);*/


	/* MC recommendation */
	if (!isCPU){
		if (k == 0)
			printf("MC: I recommend that you keep your current hand\n");
		else if (k > 0){ 
			printf("MC: I recommend that you exchange card(s): ");
			for (i = 0; i < 5; ++i){
				if (cases[k][i])
					printf("%d ", i+1);
			}
			printf("\n");
		}
		printf("MC: type 'y' to do so\n");
	}
	memcpy(result, &cases[k], sizeof(int) * 5);
	if (k == 0)
		result[0] = -1;
	return result;
}