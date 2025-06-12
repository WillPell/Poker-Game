#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "card.h"
#include "player.h"



int setupGame(Player **head); // Pass by reference
void playHand(Player **head, int *numPlayers, Player *dealer, Card deck[DECK_SIZE]);
void postBlinds(Player *dealer, int *pot, int *currentBet);
void betting(int numPlayers, Player *head, int *pot, Card communityCards[5], int numCommunityCardsDealt, int currentBet);
int checkAutoWin(Player *dealer, int *pot);
void displayPrivacyScreen(Player *currentPlayer, Card communityCards[5], int numCommunityCards, int pot, int currentBet);
void waitForEnter();

#endif