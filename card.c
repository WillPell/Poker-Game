#include "card.h"


Card createCard(char *rank, char *suit){
    Card card;
    
    // +1 for \0
    card.rank = malloc(strlen(rank) + 1);
    card.suit = malloc(strlen(suit) + 1);
    

    if(card.rank == NULL || card.suit == NULL){
        if(card.rank){
            free(card.rank);
        }
        if(card.suit){
            free(card.suit);
        }
        card.rank = NULL;
        card.suit = NULL;
        return card;
    }

    strcpy(card.rank, rank);
    strcpy(card.suit, suit);

    return card;
}


void createAndShuffleDeck(Card deck[DECK_SIZE]){
    char *ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    char *suits[] = {"H", "D", "C", "S"};

    int index = 0;
    
    for(int i = 0; i < NUM_SUITS; i++){
        for(int j = 0; j < NUM_RANKS; j++){
            deck[index] = createCard(ranks[j], suits[i]);
            index++;
        }
    }

    shuffleDeck(deck);
}

void shuffleDeck(Card deck[DECK_SIZE]){
    // Fisher yates
    for(int i = DECK_SIZE - 1; i > 0; i--){
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}


void printCard(Card *card){
    if(card == NULL || card->rank == NULL || card->suit == NULL){
        printf("Error, null pointer to cards");
        return;
    }
    char* suitColor = getSuitColor(card->suit);
    printf("%s%s%s%s", suitColor, card->rank, card->suit, RESET); // stop the entire terminal text being one color lol
}


char* getSuitColor(char *suit){
    if(suit == NULL){
        return RESET;
    }
    if(strcmp(suit, "H") == 0){
        return RED;
    }
    if(strcmp(suit, "D") == 0){
        return BLUE;
    }
    if(strcmp(suit, "C") == 0){
        return GREEN;
    }
    if(strcmp(suit, "S") == 0){
        return PURPLE;
    }
    
    printf("Suit error!");
    return RESET;
}

//let count = 3 then 4 then 5
void printCommunityCards(Card deck[], int startIndex, int count){
    printf("\nCommunity Cards: ");
    
    if(count <= 0){
        printf("None dealt yet");
    }
    else{
        for(int i = 0; i < count; i++){
            if(i > 0){
                printf(", ");}
            printCard(&deck[startIndex + i]);
        }
    }
    printf("\n");
}


// check for those NULL card issues, basically a bool
int isValidCard(Card *card){
    return(card != NULL && card->rank != NULL && card->suit != NULL);
}


// Will be replaced later sorry sam
int rankValue(char *rank) {
    if(strcmp(rank, "2") == 0){
        return 2;
    }
    if(strcmp(rank, "3") == 0){
        return 3;
    }
    if(strcmp(rank, "4") == 0){
        return 4;
    }
    if(strcmp(rank, "5") == 0){
        return 5;
    }
    if(strcmp(rank, "6") == 0){
        return 6;
    }
    if(strcmp(rank, "7") == 0){
        return 7;
    }
    if(strcmp(rank, "8") == 0){
        return 8;
    }
    if(strcmp(rank, "9") == 0){
        return 9;
    }
    if(strcmp(rank, "10") == 0){
        return 10;
    }
    if(strcmp(rank, "J") == 0){
        return 11;
    }
    if(strcmp(rank, "Q") == 0){
        return 12;
    }
    if(strcmp(rank, "K") == 0){
        return 13;
    }
    if(strcmp(rank, "A") == 0){
        return 14;
    }
    return 0;
}


void sortHand(Card hand[HAND_SIZE]){
    if(hand == NULL){
        return;
    }
    
    // swap
    if(hand[0].rank != NULL && hand[1].rank != NULL && hand[0].suit != NULL && hand[1].suit != NULL){
        if(rankValue(hand[0].rank) > rankValue(hand[1].rank)){
            Card temp = hand[0];
            hand[0] = hand[1];
            hand[1] = temp;
        }
    }
}


void freeCard(Card *card){
    if(card == NULL){
        return;
    }
    
    if(card->rank != NULL){
        free(card->rank);
        card->rank = NULL;
    }
    
    if(card->suit != NULL){
        free(card->suit);
        card->suit = NULL;
    }
}

// loops free card in whole deck
void freeDeck(Card deck[], int size) {
    for(int i = 0; i < size; i++){
        freeCard(&deck[i]);
    }
}
