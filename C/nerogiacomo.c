#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52
#define HAND_SIZE 11

typedef struct {
    char *face;
    char *suit;
    int value;
} Card;

typedef struct {
    Card cards[DECK_SIZE];
    int top;
} Deck;

typedef struct {
    Card cards[HAND_SIZE];
    int count;
    int total;
} Hand;

void initializeDeck(Deck *deck);
void shuffleDeck(Deck *deck);
Card drawCard(Deck *deck);
void addCardToHand(Hand *hand, Card card);
void printHand(Hand *hand, int hideFirstCard);
int calculateHandTotal(Hand *hand);

int main() {
    printf("Welcome to Blackjack! this game was made by ClopeziohH,\n");
    printf("You start with 100 money. You can bet any amount of money you have.\n");
    printf("If you win, you get double the money you bet. If you lose, you lose the money you bet.\n");
    printf("The goal is to get as close to 21 as possible without going over.\n");
    printf("Good luck!\n");
    
    Deck deck;
    Hand playerHand = { .count = 0, .total = 0 };
    Hand houseHand = { .count = 0, .total = 0 };
    int money = 100;
    int bet;

    srand(time(NULL));
   
    while (money > 0) {
        printf("You have %d money. How much do you want to bet? ", money);
        scanf("%d", &bet);

        if (bet > money) {
            printf("You cannot bet more than you have!\n");
            continue;
        }

        money -= bet;

        initializeDeck(&deck);
        shuffleDeck(&deck);

        playerHand.count = playerHand.total = 0;
        houseHand.count = houseHand.total = 0;

        addCardToHand(&playerHand, drawCard(&deck));
        addCardToHand(&playerHand, drawCard(&deck));
        addCardToHand(&houseHand, drawCard(&deck));
        addCardToHand(&houseHand, drawCard(&deck));

        printf("Player's hand:\n");
        printHand(&playerHand, 0);
        printf("House's hand:\n");
        printHand(&houseHand, 1);

        char choice;
        while (1) {
            printf("Do you want to draw a card? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                addCardToHand(&playerHand, drawCard(&deck));
                printf("Player's hand:\n");
                printHand(&playerHand, 0);
                if (playerHand.total > 21) {
                    printf("Player busts! Total: %d\n", playerHand.total);
                    break;
                }
            } else if (choice == 'n' || choice == 'N') {
                printf("Player's hand:\n");
                printHand(&playerHand, 0);
                printf("House's hand:\n");
                printHand(&houseHand, 0);
                while (houseHand.total < 17) {
                    addCardToHand(&houseHand, drawCard(&deck));
                    printf("House's hand:\n");
                    printHand(&houseHand, 0);
                }
                if (houseHand.total > 21) {
                    printf("House busts! Total: %d\n", houseHand.total);
                    money += 2 * bet;
                } else if (houseHand.total > playerHand.total) {
                    printf("House wins with a total of %d!\n", houseHand.total);
                } else if (houseHand.total < playerHand.total) {
                    printf("Player wins with a total of %d!\n", playerHand.total);
                    money += 2 * bet;
                } else {
                    printf("It's a tie!\n");
                    money += bet;
                }
                break;
            } else {
                printf("Invalid choice. Please enter 'y' or 'n'.\n");
            }
        }

        if (money <= 0) {
            printf("You have lost all your money. You get 10 money to continue.\n");
            money = 10;
        }
    }

    return 0;
}

void initializeDeck(Deck *deck) {
    char *faces[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    deck->top = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck->cards[deck->top].face = faces[j];
            deck->cards[deck->top].suit = suits[i];
            deck->cards[deck->top].value = values[j];
            deck->top++;
        }
    }
    deck->top = 0;
}

void shuffleDeck(Deck *deck) {
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

Card drawCard(Deck *deck) {
    return deck->cards[deck->top++];
}

void addCardToHand(Hand *hand, Card card) {
    hand->cards[hand->count++] = card;
    hand->total += card.value;
}

void printHand(Hand *hand, int hideFirstCard) {
    for (int i = 0; i < hand->count; i++) {
        if (i == 0 && hideFirstCard) {
            printf("Face Down Card\n");
        } else {
            printf("%s of %s\n", hand->cards[i].face, hand->cards[i].suit);
        }
    }
    if (!hideFirstCard) {
        printf("Total: %d\n", hand->total);
    }
}
