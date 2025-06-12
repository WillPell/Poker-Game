#ifndef RANK_H
#define RANK_H

#include "player.h"
#include "card.h"


enum HandRank {
    HighCard = 1,
    OnePair,
    TwoPair,
    Trips,
    Straight,
    Flush,
    FullHouse,
    Quads,
    StraightFlush,
    RoyalFlush
};

typedef struct {
    enum HandRank rank; // Primary value for comparison
    int value; // high card for tie break
} HandValue;

// Function prototypes
int getCardValue(Card card);
void sortByValue(Card cards[], int size);
int flush(Card cards[], int size, Card *flushCards);
int straight(Card cards[], int size, Card *straightCards);
HandValue evaluateHand(Card handCards[HAND_SIZE], Card communityCards[5]);
int compareHands(HandValue hand1, HandValue hand2);
Player* determineWinner(Player *head, Card communityCards[5]);
char* getHandRankString(enum HandRank rank);



#endif