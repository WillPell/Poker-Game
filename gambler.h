#ifndef GAMBLER_H
#define GAMBLER_H

#include "common.h"
#include "player.h"

typedef struct {
    char name[MAX_DRINK_NAME_LENGTH];
    char description[100];
    int isElimination; // 1 eliminates the player, 0 safe
} Drink;

// drinks
void initializeDrinks(Drink drinks[NUM_DRINKS]);
void drinkSystem(Player **head, int *numPlayers, Player *winner);
int blackjackMinigame(Player *player, int potWinnings);
void shuffleDrinks(Drink drinks[NUM_DRINKS]);


// blackjack
int calculateBlackjackValue(int cards[], int numCards);
void printBlackjackCard(int card);

#endif
