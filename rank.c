#include "rank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int getCardValue(Card card){
    // face card conversion
    if(strcmp(card.rank, "A") == 0){
        return 14;
    }
    if(strcmp(card.rank, "K") == 0){
        return 13;
    }
    if(strcmp(card.rank, "Q") == 0){
        return 12;
    }
    if(strcmp(card.rank, "J") == 0){
        return 11;
    }
    return atoi(card.rank); // char to int converion
}


// Sorts cards using a lil b-sort
void sortByValue(Card cards[], int size) {
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - i - 1; j++){
            if(getCardValue(cards[j]) < getCardValue(cards[j + 1])){
                Card temp = cards[j];
                cards[j] = cards[j + 1];
                cards[j + 1] = temp;
            }
        }
    }
}

// histogram flush method
int flush(Card cards[], int size, Card *flushCards){
    int suitCount[4] = {0}; 
    char* suits[4] = {"C", "D", "H", "S"};
    

    for(int i = 0; i < size; i++){
        for(int j = 0; j < 4; j++){
            if (strcmp(cards[i].suit, suits[j]) == 0){
                suitCount[j]++;
                break;
            }
        }
    }
    
    // look for 5
    for(int i = 0; i < 4; i++){
        if (suitCount[i] >= 5){
            int flushIndex = 0;
            for (int j = 0; j < size && flushIndex < 5; j++){
                if (strcmp(cards[j].suit, suits[i]) == 0){
                    flushCards[flushIndex++] = cards[j];
                }
            }
            
        return getCardValue(flushCards[0]);
        }
    }
    
    return 0;
}

// straight bitmap method
int straight(Card cards[], int size, Card *straightCards) {
    int present[15] = {0}; 
    
    for(int i = 0; i < size; i++){
        present[getCardValue(cards[i])] = 1;
    }
    
    
    for(int i = 14; i >= 6; i--){
        if(present[i] && present[i-1] && present[i-2] && present[i-3] && present[i-4]){
            int straightIndex = 0;
            int valuesNeeded[5] = {i, i-1, i-2, i-3, i-4};
            
            for(int v = 0; v < 5; v++) {
                for(int j = 0; j < size; j++) {
                    if(getCardValue(cards[j]) == valuesNeeded[v]){
                        straightCards[straightIndex++] = cards[j];
                        break;
                    }
                }
            }
            
            return i;
        }
    }
    
    
    if(present[14] && present[2] && present[3] && present[4] && present[5]) {
        int straightIndex = 0;
        int valuesNeeded[5] = {5, 4, 3, 2, 14}; //hard code low ace straight
        
        
        for(int v = 0; v < 5; v++){
            for(int j = 0; j < size; j++){
                if(getCardValue(cards[j]) == valuesNeeded[v]){
                    straightCards[straightIndex++] = cards[j];
                    break;
                }
            }
        }
        
        return 5; //hard code 5 as low ace straight high card
    }

    return 0;
}


//uses highcard>0 to determine a result from prev function check
HandValue evaluateHand(Card handCards[HAND_SIZE], Card communityCards[5]) {
    HandValue result;
    

    if(handCards == NULL || communityCards == NULL){
        result.rank = HighCard;
        result.value = 0;
        return result;
    }
    
    //combine
    Card allCards[7];
    allCards[0] = handCards[0];
    allCards[1] = handCards[1];
    for(int i = 0; i < 5; i++){
        allCards[i+2] = communityCards[i];
    }
    

    sortByValue(allCards, 7);
    
    //flush
    Card flushCards[5];
    int flushHighCard = flush(allCards, 7, flushCards);
    
    //straight
    Card straightCards[5];
    int straightHighCard = straight(allCards, 7, straightCards);
    
    //royal/straight flush checks
    if(flushHighCard > 0 && straightHighCard > 0){
        Card straightFlushCards[5];
        int straightFlushHighCard = straight(flushCards, 5, straightFlushCards);
        
        if(straightFlushHighCard > 0){
            if(straightFlushHighCard == 14) {
                result.rank = RoyalFlush;
                result.value = straightFlushHighCard;
            } 
            else{
                result.rank = StraightFlush;
                result.value = straightFlushHighCard;
            }
            return result;
        }
    }
    
    //card frequencies
    int valueCounts[15] = {0};
    for(int i = 0; i < 7; i++){
        valueCounts[getCardValue(allCards[i])]++;
    }
    
    //iterate backward until find 4 in histogram
    for(int i = 14; i >= 2; i--){
        if(valueCounts[i] == 4){
            result.rank = Quads;
            result.value = i;
            return result;
        }
    }
    
    //full house
    int threeOfAKind = 0;
    int pair = 0;
    
    //check for trips
    for(int i = 14; i >= 2; i--){
        if(valueCounts[i] >= 3){
            threeOfAKind = i;
            break;
        }
    }
    //if trips check for 2pair
    if(threeOfAKind > 0){
        for(int i = 14; i >= 2; i--){
            if(i != threeOfAKind && valueCounts[i] >= 2){
                pair = i;
                break;
            }
        }
        
        if(pair > 0){
            result.rank = FullHouse;
            result.value = threeOfAKind; //fh uses trips to tiebreak
            return result;
        }
    }
    
    //flush
    if(flushHighCard > 0){
        result.rank = Flush;
        result.value = flushHighCard;
        return result;
    }
    
    //straight
    if(straightHighCard > 0){
        result.rank = Straight;
        result.value = straightHighCard;
        return result;
    }
    
    //trips
    if(threeOfAKind > 0){
        result.rank = Trips;
        result.value = threeOfAKind;
        return result;
    }
    
    //two pair
    int firstPair = 0;
    int secondPair = 0;
    
    for(int i = 14; i >= 2; i--){
        if(valueCounts[i] == 2){
            if(firstPair == 0){
                firstPair = i;
            }
            else{
                secondPair = i;
                break;
            }
        }
    }
    
    if(firstPair > 0 && secondPair > 0){
        result.rank = TwoPair;
        result.value = firstPair; //loop iterated backward to first pair would be bigger
        return result;
    }
    
    //one pair
    if(firstPair > 0){
        result.rank = OnePair;
        result.value = firstPair;
        return result;
    }
    
    //high card
    result.rank = HighCard;
    result.value = getCardValue(allCards[0]); // sorted cards mean high card at 0
    
    return result;
}


int compareHands(HandValue hand1, HandValue hand2){
    if(hand1.rank > hand2.rank){
        return 1;
    }
    if(hand1.rank < hand2.rank){
        return -1;
    }
    
    if(hand1.value > hand2.value){
        return 1;
    }
    if(hand1.value < hand2.value){
        return -1;
    }
    
    return 0; //if full tie
}


Player* determineWinner(Player *head, Card communityCards[5]) {
    if(head == NULL || communityCards == NULL){
        return NULL;
    }
    
    Player *current = head;
    Player *winner = NULL;
    HandValue bestHand;
    bestHand.rank = 0;
    bestHand.value = 0;
    

    do {
        if (current->credits > 0 && !current->foldStatus) {
            HandValue currentHand = evaluateHand(current->hand, communityCards);
            
            printf("\n");
            printPlayerName(current);
            printf("'s hand: ");
            printCard(&current->hand[0]);
            printf(", ");
            printCard(&current->hand[1]);
            printf(" - %s\n", getHandRankString(currentHand.rank));
            
            //if no winner or if winner worse than current
            if (winner == NULL || compareHands(currentHand, bestHand) > 0) {
                winner = current;
                bestHand = currentHand;
            }
        }
        current = current->next;
    } while (current != head);
    
    return winner;
}

//returns pointer to string based on hand
char* getHandRankString(enum HandRank rank) {
    switch (rank) {
        case HighCard:
            return "High Card";
        case OnePair:
            return "One Pair";
        case TwoPair:
            return "Two Pair";
        case Trips:
            return "Trips";
        case Straight:
            return "Straight";
        case Flush:
            return "Flush";
        case FullHouse:
            return "Full House";
        case Quads:
            return "Four of a Kind";
        case StraightFlush:
            return "Straight Flush";
        case RoyalFlush:
            return "Royal Flush";
        default:
            return "Wrong";
    }
}
