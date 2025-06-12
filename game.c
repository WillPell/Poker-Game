#include "game.h"
#include "rank.h"
#include "gambler.h"


int setupGame(Player **head) {
    int numPlayers;

    // Artistic talent
    printf("   _____   _____   _____   _____\n");
    printf("  |A .  | |K .  | |Q .  | |J .  |\n");
    printf("  | /\\. | | /\\. | | /\\. | | /\\. |\n");
    printf("  |(_._)| |(_._)| |(_._)| |(_._)|\n");
    printf("  |  |  | |  |  | |  |  | |  |  |\n");
    printf("  |____V| |____K| |____Q| |____J|\n");
    printf("\n");
    printf("        W P   K E E P ' E M\n");
    printf("\n");
    
    do{
        printf("Enter number of players (2-8): ");
        if(scanf("%d", &numPlayers) != 1){ //ensures 1 int is inputed
            printf("Invalid input\n");
            int c;
            while((c = getchar()) != '\n'); //clears buffer
            numPlayers = 0;
            continue;
        }
        getchar();//eats new line

        if(numPlayers < 2 || numPlayers > 8){
            printf("Please keep the player count within 2-8\n");
        }    
    } 
    while(numPlayers < 2 || numPlayers > 8);
    
    Player *current = NULL;
    
    
    char* availableColors[] = {RED, GREEN, YELLOW, BLUE, PURPLE, GOLD, SILVER, NAVY};
    char* colorNames[] = {"Red", "Green", "Yellow", "Blue", "Purple", "Gold", "Silver", "Navy"};
    int usedColors[8] = {0}; // colour bitmap
    int numColors = 8;
    
    //name loop
    for(int i = 0; i < numPlayers; i++){
        char name[MAX_NAME_LENGTH];
        int validName = 0;
        int colorChoice = -1;
        
        do{
            printf("Enter name for player %d: ", i + 1);
            if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL) {
                continue;
            }
            
            name[strcspn(name, "\n")] = 0;//gets rid of \n
            
            if (strlen(name) > 0 && strlen(name) < MAX_NAME_LENGTH - 1) {
                validName = 1;
            } else {
                printf("Please enter a valid name.\n");
            }
        } while(!validName);
        
        printf("\n%s, choose your name color:\n", name);
        for(int j = 0; j < numColors; j++){
            if(!usedColors[j]){
                printf("%d: %s%s%s\n", j + 1, availableColors[j], colorNames[j], RESET);
            }
        }
        
        int validColor = 0;
        do{
            printf("Enter color choice (1-%d): ", numColors); //keep like this until happy with colours
            if(scanf("%d", &colorChoice) != 1){
                printf("Invalid input. Please enter a number.\n");
                
                int c;
                while ((c = getchar()) != '\n');
                continue;
            }
            getchar();
            
            //add to used
            if(colorChoice >= 1 && colorChoice <= numColors && !usedColors[colorChoice - 1]){
                validColor = 1;
                usedColors[colorChoice - 1] = 1;
            } 
            else if(usedColors[colorChoice - 1]){
                printf("Someone already chose that color\n");
            } 
            else{
                printf("Please enter a number between 1 and %d.\n", numColors);
            }
        } while(!validColor);

        Player *newPlayer = createPlayer(name, STARTING_CREDITS, availableColors[colorChoice - 1]);
        
        if(newPlayer == NULL){
            printf("Player making error!\n");
            if(*head != NULL){
                freePlayers(*head);
                *head = NULL;
            }
            return 0;
        }

        //makes new player head or adds to LL
        if(*head == NULL){
            *head = newPlayer;
        } 
        else{
            current->next = newPlayer;
            newPlayer->prev = current;
        }
        current = newPlayer;
        
        printf("Welcome, ");
        printPlayerName(newPlayer);
        printf("!\n\n");
    }

    //makes circular
    if(current != NULL){
        current->next = *head;
        (*head)->prev = current;
    }

    return numPlayers;
}

void playHand(Player **head, int *numPlayers, Player *dealer, Card deck[DECK_SIZE]){
    int cardIndex = 0;
    int pot = 0;
    int currentBet = 0;

    
    if(head == NULL || *head == NULL || dealer == NULL || deck == NULL || *numPlayers < 2){
        printf("Error, in player/deck data");
        return;
    }

    shuffleDeck(deck);
    
    int activePlayers = 0;
    Player *current = dealer;
    do{
        if(current->credits > 0){
            activePlayers++;
            current->foldStatus = 0;
            current->hasActed = 0;
            current->currentBet = 0;
        }
        current = current->next;
    }while(current != dealer);
    
    if (activePlayers < 2){
        return;
    }
    current = dealer; //set back to dealer
    do {
        if(current->credits > 0){
            initialiseHand(current->hand);
            
            for(int i = 0; i < HAND_SIZE; i++){
                add(current->hand, deck[cardIndex++]);
            }
            sortHand(current->hand);
            printf("\n");
            printPlayerName(current);
            printf(": ");
            printCard(&current->hand[0]);
            printf(", ");
            printCard(&current->hand[1]);
            printf("\n");
        }   
    current = current->next;
    } while(current != dealer);

    //pass address to modify original variables
    postBlinds(dealer, &pot, &currentBet);

    //round one no commcards
    betting(*numPlayers, dealer, &pot, NULL, 0, currentBet);
    if(checkAutoWin(dealer, &pot)){
        return;
    }

    //flop
    printCommunityCards(deck, cardIndex, 3);
    cardIndex += 3;
    betting(*numPlayers, dealer, &pot, &deck[cardIndex - 3], 3, 0);
    if(checkAutoWin(dealer, &pot)){
        return;
    }

    //turn
    printCommunityCards(deck, cardIndex - 3, 4);
    cardIndex++;
    betting(*numPlayers, dealer, &pot, &deck[cardIndex - 4], 4, 0);
    if(checkAutoWin(dealer, &pot)) {
        return;
    }

    //river
    printCommunityCards(deck, cardIndex - 4, 5);
    cardIndex++;
    betting(*numPlayers, dealer, &pot, &deck[cardIndex - 5], 5, 0);
    if(checkAutoWin(dealer, &pot)){
        return;
    }

    Card communityCards[5];

    int firstCommunityCardIndex = cardIndex - 5;
    for(int i = 0; i < 5; i++){
        communityCards[i] = deck[firstCommunityCardIndex + i];
    }
    printf("\nPress Enter to continue to results...");
    int c;
    while ((c = getchar()) != '\n');
    getchar();
    system("cls");
    
    
    printf("\n================ RESULTS ================\n");
    printf("\nTotal pot: %d\n", pot);
    printf("\n========================================\n");

    Player* winner = determineWinner(dealer, communityCards);
      if(winner != NULL){
        HandValue winningHand = evaluateHand(winner->hand, communityCards);
        printf("\n================ HAND COMPLETE ================\n");
        printf("\n");
        printPlayerName(winner);
        printf(" wins the pot of %d with %s!\n", pot, getHandRankString(winningHand.rank));

        int finalWinnings = blackjackMinigame(winner, pot);
        winner->credits += finalWinnings;
        printf("\n==============================================\n");
        waitForEnter();
        
        system("cls");
             
        drinkSystem(head, numPlayers, winner);
    } 
    else{        
        printf("\nNo active players remaining. Pot remains.\n");
        waitForEnter();
        system("cls");
    }
}

void postBlinds(Player *dealer, int *pot, int *currentBet) {
    if (dealer == NULL || pot == NULL || currentBet == NULL) {
        return;
    }
    
    Player *smallBlind = dealer->next;
    while(smallBlind->credits <= 0){
        smallBlind = smallBlind->next;
        
        if(smallBlind == dealer){
            return;
        }
    }
    
    
    Player *bigBlind = smallBlind->next;
    while(bigBlind->credits <= 0){
        bigBlind = bigBlind->next;
        
        if(bigBlind == dealer){
            return;
        }
    }
    //checks if player has enough credits, if not basically all ins them lol
    int smallBlindAmount = (smallBlind->credits >= SMALL_BLIND) ? SMALL_BLIND : smallBlind->credits;
    smallBlind->credits -= smallBlindAmount;
    smallBlind->currentBet = smallBlindAmount;
    smallBlind->hasActed = 0;

    
    printf("\n");
    printPlayerName(smallBlind);
    printf(" posts small blind of %d\n", smallBlindAmount);
    
    
    displayPrivacyScreen(smallBlind, NULL, 0, *pot, smallBlindAmount);
    printPlayerName(smallBlind);
    printf(" - You have put in the small blind of %d\n", smallBlindAmount);
    printf("Press Enter to continue...");
    int c;
    while ((c = getchar()) != '\n');
    getchar();
    
    
    int bigBlindAmount = (bigBlind->credits >= BIG_BLIND) ? BIG_BLIND : bigBlind->credits;
    bigBlind->credits -= bigBlindAmount;
    bigBlind->currentBet = bigBlindAmount;
    bigBlind->hasActed = 0; 
    *pot += bigBlindAmount;
    *currentBet = bigBlindAmount;
    printPlayerName(bigBlind);
    printf(" posts big blind of %d\n", bigBlindAmount);
    
    
    displayPrivacyScreen(bigBlind, NULL, 0, *pot, bigBlindAmount);
    printPlayerName(bigBlind);
    printf(" - You have put in the big blind of %d\n", bigBlindAmount);
    printf("Press Enter to continue...");
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
    
    printf("Total pot after blinds: %d\n\n", *pot);
    
    
    Player *current = dealer;
    do {
        if (current->credits > 0) {
            current->hasActed = 0;
        }
        current = current->next;
    } while (current != dealer);
}

void betting(int numPlayers, Player *head, int *pot, Card communityCards[5], int numCommunityCardsDealt, int currentBet) {
    Player *current = head;
    int bettingComplete = 0;
    

    //once blinds r in
    if(currentBet > 0) { 
        //small blind
        current = head->next; 
        while(current->credits <= 0) {
            current = current->next;
        }
        
        //big blind
        current = current->next;
        while(current->credits <= 0) {
            current = current->next;
        }
        
        //first to bet
        current = current->next;
        while(current->credits <= 0 && current != head) {
            current = current->next;
        }
    }
    else{
        //resets
        Player *resetCurrent = head;
        do{
            if(resetCurrent->credits > 0 && !resetCurrent->foldStatus){
                resetCurrent->hasActed = 0;
                resetCurrent->currentBet = 0;
            }
            resetCurrent = resetCurrent->next;
        } while(resetCurrent != head);
        
       
        while(current->credits <= 0 || current->foldStatus){
            current = current->next;
            if(current == head) break;
        }
    }
    
    while(!bettingComplete){
        if(current->credits > 0 && !current->foldStatus){ 
            if(!current->hasActed || (currentBet > current->currentBet)) {
                //if they have creds, havent folded or acted OR if their bet has been raised on
                displayPrivacyScreen(current, communityCards, numCommunityCardsDealt, *pot, currentBet);
                char action;
                int validInput = 0;                  
                do{
                    printf("\n");
                    printPlayerName(current);
                    printf(" - Balance: %d\nCurrent bet to call: %d\n\nF for Fold\nC for Call\nR for Raise\nAction: ", current->credits, currentBet - current->currentBet);
                    if (scanf(" %c", &action) != 1) { // ensures one char is entered
                        printf("Invalid input. Please try again.\n");
                        
                        int c;
                        while ((c = getchar()) != '\n');
                        continue;
                    }
                    
                    
                    int c;
                    while ((c = getchar()) != '\n');
                    validInput = 1;

                } while(!validInput);
                  
                switch (action){
                    case 'F': case 'f': 
                        printPlayerName(current);
                        printf(" folds.\n");
                        current->foldStatus = 1;
                        current->hasActed = 1;
                        break;
                        
                    case 'C': case 'c':
                    {
                        int callAmount = currentBet - current->currentBet;
                        if(callAmount > 0) {
                            if(callAmount > current->credits){
                                printf("Too broke to call. You have to all-in :o\n");
                                printPlayerName(current);
                                printf(" goes all-in with %d.\n", current->credits);
                                *pot += current->credits;
                                current->currentBet += current->credits;
                                current->credits = 0;
                            } 
                            else{
                                printPlayerName(current);
                                printf(" calls with %d.\n", callAmount);
                                *pot += callAmount;
                                current->credits -= callAmount;
                                current->currentBet = currentBet;
                            }
                        } 
                        else{
                            printPlayerName(current);
                            printf(" checks.\n");
                        }
                        current->hasActed = 1;
                        break;
                    }

                    case 'R': case 'r':
                    {
                        int raiseAmount;
                        int validRaise = 0;
                        do{
                            printPlayerName(current);
                            printf(" - Enter total raise amount (minimum %d): ", currentBet + 1);
                            if(scanf("%d", &raiseAmount) != 1){
                                printf("Enter number please\n");
                                int c;
                                while ((c = getchar()) != '\n');
                                continue; //restart loop
                            }
                            
                            if(raiseAmount <= currentBet){
                                printf("Raise must be greater than current bet (%d)\n", currentBet);
                                continue;
                            }
                            
                            int totalNeeded = raiseAmount - current->currentBet;
                            if(totalNeeded > current->credits){
                                printf("You don't have enough credits. Maximum you can bet is %d\n", current->credits + current->currentBet);
                                continue;
                            }
                            validRaise = 1;
                        } while(!validRaise);


                        int additionalBet = raiseAmount - current->currentBet;
                        printPlayerName(current);
                        printf(" raises to %d (+%d).\n", raiseAmount, additionalBet);
                        currentBet = raiseAmount;
                        *pot += additionalBet;
                        current->credits -= additionalBet;
                        current->currentBet = raiseAmount;
                        current->hasActed = 1;
                        
                        //back round again
                        Player *resetPlayer = head;
                        do{
                            if(resetPlayer != current && resetPlayer->credits > 0 && !resetPlayer->foldStatus) {
                                resetPlayer->hasActed = 0;
                            }
                            resetPlayer = resetPlayer->next;
                        } while (resetPlayer != head);
                        break;
                        }
                    default:
                        printf("Invalid action\n");
                        continue;
                }
            }
        }
        
        
        current = current->next;
        int playersNeedingToAct = 0;
        Player *checkPlayer = head;
        //checks everyone has acted
        do{
            if(checkPlayer->credits > 0 && !checkPlayer->foldStatus){
                if(!checkPlayer->hasActed || (currentBet > checkPlayer->currentBet && checkPlayer->credits > 0)){
                    playersNeedingToAct++;
                }
            }
            checkPlayer = checkPlayer->next;
        } while (checkPlayer != head);
          
        if(playersNeedingToAct == 0){
            bettingComplete = 1; //breaks the loop if everyone has acted
        }
    }
}


int checkAutoWin(Player *dealer, int *pot){
    int activePlayers = 0;
    Player *winner = NULL;
    Player *current = dealer;
    
    
    do{
        if(current->credits > 0 && !current->foldStatus){
            activePlayers++;
            winner = current;
        }
        current = current->next;
    } while (current != dealer);

    if (activePlayers == 1) {        
        printf("\n================ HAND COMPLETE ================\n");
        printf("\n");
        printPlayerName(winner);
        printf(" wins the pot of %d as the last player remaining!\n", *pot);
        winner->credits += *pot;
        printf("\n==============================================\n");
        waitForEnter();
        
        system("cls");
        
        return 1;
    }
    
    return 0;
}


void displayPrivacyScreen(Player *currentPlayer, Card communityCards[5], int numCommunityCards, int pot, int currentBet){
    
    system("cls");
    printf("\n====================");
    printPlayerName(currentPlayer);
    printf("'s Turn ====================\n\n");
    
    printf("Your Cards: ");
    for(int i = 0; i < HAND_SIZE; i++){
        printCard(&currentPlayer->hand[i]);

        if(i < HAND_SIZE - 1){
            printf(", ");
        }
    }
    printf("\n\n");
    
    
    printf("Community Cards: ");
    if(numCommunityCards == 0){
        printf("Not dealt yet");
    } 
    else{
        for(int i = 0; i<numCommunityCards; i++) {
            printCard(&communityCards[i]);
            if (i < numCommunityCards - 1){
                printf(", ");}
        }
    }
    printf("\n\n");
    
    
    printf("Current Pot: %d\n", pot);
    printf("Current Bet: %d\n", currentBet);
    printf("Your Current Bet: %d\n", currentPlayer->currentBet);
    printf("Amount to Call: %d\n", currentBet - currentPlayer->currentBet);
    printf("Your Balance: %d\n", currentPlayer->credits);
    
    
    if(currentPlayer->foldStatus){
        printf("\nYou have folded this hand.\n");
    }
    printf("\n==========================================================\n\n");
}


void waitForEnter() {
    int c;
    while ((c = getchar()) != '\n');
    printf("Press Enter to continue...");
    getchar();
}