#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "card.h"

typedef struct Player {
    char *name; 
    char nameColor[COLOR_CODE_LENGTH]; 
    int credits;
    int foldStatus; // these two are basically python bools
    int hasActed;
    int currentBet;
    struct Player *next;
    struct Player *prev;
    Card hand[HAND_SIZE];
}Player;



Player *createPlayer(char *name, int credits, char *nameColor);
void freePlayers(Player *head);
void initialiseHand(Card hand[HAND_SIZE]);
int add(Card hand[HAND_SIZE], Card card);
void sortHand(Card hand[HAND_SIZE]);
void printPlayerName(Player *player);


#endif