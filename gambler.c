#include "gambler.h"
#include <time.h>

void initializeDrinks(Drink drinks[NUM_DRINKS]){
    
    strcpy(drinks[0].name, "Great Northern");
    strcpy(drinks[0].description, "Cheap but very average");
    drinks[0].isElimination = 0;
    
    strcpy(drinks[1].name, "XXXX");
    strcpy(drinks[1].description, "Tried and true");
    drinks[1].isElimination = 0;
    
    strcpy(drinks[2].name, "Pear Sommersby");
    strcpy(drinks[2].description, "Fruity and refreshing");
    drinks[2].isElimination = 0;
    
    strcpy(drinks[3].name, "Johnnie Walker & Coke");
    strcpy(drinks[3].description, "For the high rollers");
    drinks[3].isElimination = 0;
    
    strcpy(drinks[4].name, "House Special");
    strcpy(drinks[4].description, "Do NOT drink this lol");
    drinks[4].isElimination = 0;
    
    //randomly make one drink evil
    int eliminationDrink = rand() % NUM_DRINKS;
    drinks[eliminationDrink].isElimination = 1;
}

//fisher yates algorithm
void shuffleDrinks(Drink drinks[NUM_DRINKS]){
    for(int i = NUM_DRINKS - 1; i > 0; i--){
        int j = rand() % (i + 1);
        Drink temp = drinks[i];
        drinks[i] = drinks[j];
        drinks[j] = temp;
    }
}


void drinkSystem(Player **head, int *numPlayers, Player *winner){
    if(*head == NULL || winner == NULL || *numPlayers <= 1){
        return;
    }
    
    
    int losingPlayerCount = 0;
    Player *current = *head;
    
    //goes thru players to find number of losers
    do{
        if(current->credits > 0 && !current->foldStatus && current != winner) {
            losingPlayerCount++;
        }
        current = current->next;
    } while(current != *head);
    
    if(losingPlayerCount == 0){
        return;
    }
    
    printf("\n ========== GAMBLERS BAR TIME! ========== \n");
    printf("Choose very wisely :)\n\n");
    
    //pointer to array of pointers to players
    Player **losingPlayers = malloc(losingPlayerCount * sizeof(Player*));
    int index = 0;
    current = *head;
    
    //finds the losing players
    do{
        if(current->credits > 0 && !current->foldStatus && current != winner){
            losingPlayers[index++] = current;
        }
        current = current->next;
    } while(current != *head);
    
    //checks losers are still in game
    for(int i = 0; i < losingPlayerCount; i++){
        current = losingPlayers[i];
        
        
        Player *temp = *head;
        int playerStillExists = 0;
        if(*head != NULL){
            do{
                if(temp == current){
                    playerStillExists = 1;
                    break;
                }
                temp = temp->next;
            } while(temp != *head);
        }
        
        if(!playerStillExists) {
            continue;
        }
        
        //make drink structs
        Drink drinks[NUM_DRINKS];
        initializeDrinks(drinks);
        shuffleDrinks(drinks);
        
        printf("========================================\n");
        printPlayerName(current);
        printf("'s turn to drink!\n\n");
        printf("The bartender presents 5 drinks:\n\n");        
        for(int j = 0; j < NUM_DRINKS; j++){
            printf("%d. %s%s%s - %s\n", j + 1, GOLD, drinks[j].name, RESET, drinks[j].description);
        }
        
        printf("\n");
        printPlayerName(current);
        printf(", choose your drink (1-5): ");
        
        int choice;
        int validChoice = 0;
        
        do {
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input");
                int c;
                while ((c = getchar()) != '\n');
                continue;
            }
            
            if (choice >= 1 && choice <= NUM_DRINKS) {
                validChoice = 1;
            } else {
                printf("Choose a number 1-5: ");
            }
        } while (!validChoice);
        
        choice--;//account for indexing
        
        printf("\n");
        printPlayerName(current);        
        printf(" chose %s%s%s...\n\n", GOLD, drinks[choice].name, RESET);
        
        printf("...\n");
        printf("Press Enter to see what happens...");
        int c;
        while ((c = getchar()) != '\n');
        getchar();
          if (drinks[choice].isElimination) {
            printf("%sBad Drink%s ", RED, RESET);
            printPlayerName(current);
            printf(" has been %sELIMINATED%s by the %s!\n", RED, RESET, drinks[choice]);
            printf("The drink was too gross they had to leave the table!\n\n");
            

            Player *toRemove = current;
            if(toRemove->next == toRemove){
                *head = NULL;
            }
            else{
                //sets prev and next nodes to exclude itself
                toRemove->prev->next = toRemove->next;
                toRemove->next->prev = toRemove->prev;
                if(toRemove == *head){
                    *head = toRemove->next;
                }
            }

            if(toRemove->name != NULL){
            free(toRemove->name);
            }
            for(int j = 0; j < HAND_SIZE; j++) {
                freeCard(&toRemove->hand[j]);
            }
            free(toRemove);
            
            (*numPlayers)--;
            
            if (*numPlayers <= 1) {
                printf("Game over! Only one player remains!\n");
                break;
            }        } else {

            printf("%sLucky choice!%s ", GREEN, RESET);
            printPlayerName(current);
            printf(" survives! The %s went down smoothly.\n\n", drinks[choice].name);        }
        printf("Press Enter to continue...");
        getchar();
        system("cls");
    }
    
    //free temp array
    free(losingPlayers);
}

int calculateBlackjackValue(int cards[], int numCards){
    int value = 0;
    int aces = 0; //need to adjust for high/low
    
    //determines card values based on input array
    for(int i = 0; i < numCards; i++){
        int cardValue = cards[i]; //
        if(cardValue == 1){
            aces++;
            value += 11;
        }
        else if(cardValue > 10){
            value += 10;
        } 
        else{
            value += cardValue;
        }
    }
    
    while(value > 21 && aces > 0){
        value -= 10;
        aces--;
    }
    
    return value;
}

void printBlackjackCard(int card) {
    if(card == 1){
        printf("A");
    } 
    else if (card == 11){
        printf("J");
    }
    else if (card == 12){
        printf("Q");
    } 
    else if (card == 13){
        printf("K");
    }
    else {
        printf("%d", card);
    }
}

int blackjackMinigame(Player *player, int potWinnings) {
    system("cls");
    printf("\n ========== BLACKJACK BONUS! ========== \n\n");
    printPlayerName(player);
    printf(" won %d credits!\n", potWinnings);
    printf("Would you like to risk it all in a round of Blackjack?\n");
    printf("Win: Double your earnings (%d credits)\n", potWinnings * 2);
    printf("Lose: Keep nothing from this hand\n\n");
    printf("Play Blackjack? (Y/N): ");
    
    char choice;
    int validInput = 0;
    
    do{
        if(scanf(" %c", &choice) != 1){
            printf("Invalid input");
            int c;
            while((c = getchar()) != '\n');
            continue;
        }
        
        int c;
        while ((c = getchar()) != '\n');
        
        if (choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n') {
            validInput = 1;
        } 
        else{
            printf("Please enter Y or N: ");
        }
    } while(!validInput);
    
    if(choice == 'N' || choice == 'n'){
        printf("\n");
        printPlayerName(player);
        printf(" plays it safe and keeps the %d credits (nerd).\n", potWinnings);
        return potWinnings;
    }
    
    
    printf("\n Starting Blackjack game...\n\n");
    
    
    int playerCards[10];// there is NO way it goes over this
    int playerCardCount = 0;
    
    
    int dealerCards[10];
    int dealerCardCount = 0;
    
    //deal first two cards to btoh
    playerCards[playerCardCount++] = (rand() % 13) + 1;
    playerCards[playerCardCount++] = (rand() % 13) + 1;
    dealerCards[dealerCardCount++] = (rand() % 13) + 1;
    dealerCards[dealerCardCount++] = (rand() % 13) + 1;
    
    int playerValue = calculateBlackjackValue(playerCards, playerCardCount);
    int dealerValue = calculateBlackjackValue(dealerCards, dealerCardCount);
    
    
    printf("Your cards: ");
    for(int i = 0; i < playerCardCount; i++){
        printBlackjackCard(playerCards[i]);
        if(i < playerCardCount - 1){
        printf(", ");}
    }
    printf(" (Value: %d)\n", playerValue);
    
    printf("Dealer shows: ");
    printBlackjackCard(dealerCards[0]);
    printf(", [Face down card]\n\n");
    
    if(playerValue == 21){
        printf(" %sBLACKJACK!%s Nat 21!\n", GREEN, RESET);
        if(dealerValue == 21){
            printf("But the dealer also has blackjack (rigged casino lol)\n");
            printf("You keep your original winnings: %d credits\n", potWinnings);
            return potWinnings;
        } 
        else{
            printf("You win! Double winnings: %s%d credits%s\n", GREEN, potWinnings * 2, RESET);
            return potWinnings * 2;
        }
    }
    

    while(playerValue < 21){
        printf("Your current value: %d\n", playerValue);
        printf("Hit (H) or Stand (S)? ");
        
        char playerChoice;
        validInput = 0;
        
        do{
            if(scanf(" %c", &playerChoice) != 1){
                printf("Invalid input. ");
                int c;
                while((c = getchar()) != '\n');
                continue;
            }
            
            int c;
            while ((c = getchar()) != '\n');
            
            if (playerChoice == 'H' || playerChoice == 'h' || 
                playerChoice == 'S' || playerChoice == 's') {
                validInput = 1;
            } 
            else{
                printf("Invalid input");
            }
        } while(!validInput);
        
        if (playerChoice == 'S' || playerChoice == 's') {
            break;
        }
        
        //hitting
        playerCards[playerCardCount++] = (rand() % 13) + 1;
        playerValue = calculateBlackjackValue(playerCards, playerCardCount);
        
        printf("\nYou drew: ");
        printBlackjackCard(playerCards[playerCardCount - 1]);
        printf("\nYour cards: ");
        for(int i = 0; i < playerCardCount; i++){
            printBlackjackCard(playerCards[i]);
            if(i < playerCardCount - 1){
                printf(", ");
            }
        }
        printf(" (Value: %d)\n\n", playerValue);
    }
      //bust?
    if (playerValue > 21){
        printf("%sBUST!%s Over 21...\n", RED, RESET);
        printf("Better luck next time\n\n");
        return 0;
    }
    
    // Dealer turn
    printf("Dealer reveals: ");
    for(int i = 0; i < dealerCardCount; i++){
        printBlackjackCard(dealerCards[i]);
        if(i < dealerCardCount - 1){
            printf(", ");
        }
    }
    printf(" (Value: %d)\n\n", dealerValue);
    
    //hits on 16 and below and stands on 17 and above
    while (dealerValue < 17) {
        dealerCards[dealerCardCount++] = (rand() % 13) + 1;
        dealerValue = calculateBlackjackValue(dealerCards, dealerCardCount);
        
        printf("Dealer draws: ");
        printBlackjackCard(dealerCards[dealerCardCount - 1]);
        printf("\nDealer cards: ");

        for(int i = 0; i < dealerCardCount; i++){
            printBlackjackCard(dealerCards[i]);
            if(i< dealerCardCount - 1){
                printf(", ");}
        }
        printf(" (Value: %d)\n\n", dealerValue);
    }
    //showdown
    if(dealerValue > 21){
        printf("%sDealer busts!%s You win :D\n", GREEN, RESET);
        printf("Final winnings: %s%d credits%s\n\n", GREEN, potWinnings * 2, RESET);
        return potWinnings * 2;

    } 
    else if(playerValue > dealerValue){
        printf("%sYou win!%s (%d vs %d)\n", GREEN, RESET, playerValue, dealerValue);
        printf("Final winnings: %s%d credits%s\n\n", GREEN, potWinnings * 2, RESET);
        return potWinnings * 2;

    }
    else if(playerValue == dealerValue){
        printf("%sDamn%s It's a tie (%d vs %d)\n", YELLOW, RESET, playerValue, dealerValue);
        printf("You can keep your winnings: %d credits\n\n", potWinnings);
        return potWinnings;
    } 
    else {
        printf("%sDealer wins%s (%d vs %d)\n", RED, RESET, dealerValue, playerValue);
        printf("Better luck next time </3\n\n");
        return 0;
    }
}
