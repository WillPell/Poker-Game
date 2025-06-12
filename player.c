#include "player.h"


// Pass arrays in by reference, ints by value
Player *createPlayer(char *name, int credits, char *nameColor) {
    if(name == NULL || strlen(name) == 0 || credits < 0 || nameColor == NULL) {
        return NULL;
    }
    
    Player *newPlayer = malloc(sizeof(Player));
    if(newPlayer == NULL){
        return NULL;
    }

    newPlayer->name = malloc(strlen(name) + 1);
    
    if(newPlayer->name == NULL){
        free(newPlayer);
        return NULL;
    }
      
    strcpy(newPlayer->name, name);
    strcpy(newPlayer->nameColor, nameColor);
    newPlayer->credits = credits;
    newPlayer->foldStatus = 0;
    newPlayer->hasActed = 0;
    newPlayer->currentBet = 0;
    newPlayer->next = NULL;
    newPlayer->prev = NULL;
    
    initialiseHand(newPlayer->hand);
    
    return newPlayer;
}


void freePlayers(Player *head){
    if(head == NULL){
        return;
    }
    
    Player *current = head;
    Player *nextPlayer;

    // Break circular list
    if(head->next != head){
        head->prev->next = NULL;
    }

    //loops thru until current NULLs
    while(current != NULL){
        nextPlayer = current->next;
          if(current->name != NULL){
            free(current->name);
        }
        
        // Free cards from players hand
        for (int i = 0; i < HAND_SIZE; i++) {
            freeCard(&current->hand[i]);
        }
        
        free(current);
        current = nextPlayer;
    }
}

// Week 3: Empty hand
void initialiseHand(Card hand[HAND_SIZE]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        hand[i].rank = NULL;
        hand[i].suit = NULL;
    }
}

// Week 3: Add
int add(Card hand[HAND_SIZE], Card card){
    if(hand == NULL){
        return 0;
    }
    
    for(int i = 0; i < HAND_SIZE; i++){
        if(hand[i].rank == NULL && hand[i].suit == NULL){
            hand[i] = card;
            return 1;
        }
    }    
    return 0; // Hand full
}

void printPlayerName(Player *player){
    if(player == NULL || player->name == NULL){
        printf("Error with the player");
        return;
    }
    printf("%s%s%s", player->nameColor, player->name, RESET);
}