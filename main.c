#include "common.h"
#include "card.h"
#include "player.h"
#include "game.h"

int main() {
    
    srand(time(NULL));    
    int numPlayers = 0; 
    Player *head = NULL;
    Card deck[DECK_SIZE];
    numPlayers = setupGame(&head); //intialised LL
    
    if(numPlayers == 0 || head == NULL){
        printf("Game setup/null pointer error\n");
        return 1;
    }
    
    createAndShuffleDeck(deck);
    Player *dealer = head;    
    
    while(1){
        Player *current = head;
        
        do {
            Player *toRemove = NULL;
            if(current->credits <= 0){
                printPlayerName(current);
                printf(" has lost all his money and is eliminated.\n");
                toRemove = current;
                
                //1 left
                if(toRemove->next == toRemove){
                    head = NULL;
                    break;
                } 
                else{
                    toRemove->prev->next = toRemove->next;
                    toRemove->next->prev = toRemove->prev;
                    if(toRemove == head){
                        head = toRemove->next; 
                    }              
                    if(toRemove == dealer){
                        dealer = toRemove->next;
                    }
                }

                current = toRemove->next;
            
                //frees
                if(toRemove->name != NULL){
                    free(toRemove->name);
                }
                for(int i = 0; i < HAND_SIZE; i++){
                    freeCard(&toRemove->hand[i]);
                }

                free(toRemove);
                numPlayers--;
                
                if (head == NULL){
                    break;
                }
                continue;
            }
            //reset stuff for new hand
            current->foldStatus = 0;
            current->hasActed = 0;
            current->currentBet = 0;
            current = current->next;
        } while(current != head && head != NULL);
        // End if one player
        if (numPlayers <= 1) {
            break;
        }
        
        playHand(&head, &numPlayers, dealer, deck);
        
        printf("\n================ END OF HAND ================\n");
        
        current = head;
        int playersAsked = 0;
        int totalActivePlayers = 0;
        
        //first count total active players
        Player *counter = head;
        do{
            if(counter->credits > 0){
                totalActivePlayers++;
            }
            counter = counter->next;
        } while(counter != head && head != NULL);


        while(playersAsked < totalActivePlayers && head != NULL){
            if (current->credits > 0) {
                char quitResponse;
                int validInput = 0;
                  do {
                    printPlayerName(current);
                    printf(", do you want to quit? (Y/N): ");                    
                    if(scanf(" %c", &quitResponse) != 1){
                        printf("Invalid input. Please enter Y or N.\n");
                        //clear input buffer
                        int c;
                        while((c = getchar()) != '\n');
                    }
                    
                    
                    int c;
                    while ((c = getchar()) != '\n');
                    
                    if(quitResponse == 'Y' || quitResponse == 'y' || quitResponse == 'N' || quitResponse == 'n'){
                        validInput = 1;
                    } 
                    else{
                        printf("Invalid input. Please enter Y or N.\n");
                    }
                }while (!validInput);
                
                playersAsked++;
                if(quitResponse == 'Y' || quitResponse == 'y'){
                    printPlayerName(current);
                    printf(" has quit the game.\n");

                    // Remove player
                    Player *toRemove = current;
                    if (toRemove->next == toRemove) { 
                        head = NULL;
                        break;
                    } else{
                        toRemove->prev->next = toRemove->next;
                        toRemove->next->prev = toRemove->prev;
                        if (toRemove == head) {
                            head = toRemove->next;
                        }
                        if (toRemove == dealer) {
                            dealer = toRemove->next;
                        }
                        current = toRemove->next;
                    }
                      if (toRemove->name != NULL) {
                        free(toRemove->name);
                    }                    
                    for (int i = 0; i < HAND_SIZE; i++) {
                        freeCard(&toRemove->hand[i]);
                    }
                    free(toRemove);
                    
                    numPlayers--;
                    continue;
                }
            }
            current = current->next;
        }
          if (numPlayers <= 1) {
            system("cls");
            
            printf("\n===============================================\n");
            printf("           GAME OVER - CONGRATULATIONS!       \n");
            printf("===============================================\n");
            printf("\nCongrats last player standing!\n");
            printf("===============================================\n\n");
            break;
        }

        printf("\nAll players have made their decision. Starting next hand...\n\n");

        do {
            dealer = dealer->next;
        } while (dealer->credits <= 0);
    }
    freePlayers(head);
    freeDeck(deck, DECK_SIZE);
    return 0;
}
