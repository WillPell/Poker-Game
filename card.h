#ifndef CARD_H
#define CARD_H

#include "common.h"


typedef struct{
    char *rank;
    char *suit;
}Card;


// prototypes
Card createCard(char *rank, char *suit);
void freeCard(Card *card);
void freeDeck(Card deck[], int size);
void printCard(Card *card);
char* getSuitColor(char *suit);
int rankValue(char *rank);
void createAndShuffleDeck(Card deck[DECK_SIZE]);
void shuffleDeck(Card deck[DECK_SIZE]);
void printCommunityCards(Card deck[], int startIndex, int count);
void sortHand(Card hand[HAND_SIZE]);
int isValidCard(Card *card);


#endif