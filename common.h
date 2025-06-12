// Base header file for this project
// Has the necessary includes and constants
// NO STRUCTS 


//Guards
#ifndef COMMON_H
#define COMMON_H

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Player constants
#define MAX_PLAYERS 8
#define MAX_NAME_LENGTH 50
#define HAND_SIZE 2
#define STARTING_CREDITS 2000
#define NUM_DRINKS 5 // For gamblers bar
#define MAX_DRINK_NAME_LENGTH 30
#define COLOR_CODE_LENGTH 15

// Game constants/macros
#define NUM_SUITS 4
#define NUM_RANKS 13
#define RIVER_CARDS 5
#define DECK_SIZE (NUM_SUITS * NUM_RANKS)
#define SMALL_BLIND 25
#define BIG_BLIND 50
#define RANK_LENGTH 4
#define SUIT_LENGTH 2

// Colours ANSI
#define RESET "\033[0m"
#define RED "\033[31m" // Hearts
#define BLUE "\033[34m" // Diamonds
#define GREEN "\033[32m" // Clubs
#define YELLOW "\033[33m"
#define PURPLE "\033[35m" // Spades
#define GOLD "\033[33;1m"
#define SILVER "\033[37;1m"
#define BRONZE "\033[33;2m"
#define NAVY "\033[34;1m" // Maybe remove, looks the exact same to blue lol

#endif