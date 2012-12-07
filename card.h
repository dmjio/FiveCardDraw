#ifndef __CARD_H_
#define __CARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> 
#include <wchar.h> 
#include "card.h"

/* these should never change */
#define DECK_SIZE 52
#define VALUE_SIZE 13

/* card combos */
enum VALUE { Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
enum SUITE { Hearts = 0, Spades, Diamonds, Clubs };

/* card definition */
typedef struct {
	int value;
	int suite;
	char * (*to_suite)(int);
	char * (*to_val)(int);
} Card;

/* interfaces */
void print(Card * , int );
void sort_cards(Card * , int );
int card_comparator(const void * , const void * );
wchar_t * to_suite(int );
char * to_val(int );
char * to_card(Card );
void print(Card * , int);

#endif